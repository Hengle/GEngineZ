#include "DX12Shader.h"
#include "DX12Device.h"
#include "DX12Buffer.h"
#include "DX12Texture.h"
#include "DX12Executor.h"

namespace z {

// ==== DX12ShaderStage ====
struct StageEntryInfo {
	const char* entry;
	const char* target;
};

DX12ShaderStage* DX12ShaderStage::FromCompile(const char* data, size_t dataLen, ERHIShaderStage stage) {
	static std::unordered_map<ERHIShaderStage, StageEntryInfo> entryInfo = {
		{SHADER_STAGE_VERTEX, {"VS", "vs_5_0"}},
		{SHADER_STAGE_PIXEL, {"PS", "ps_5_0"}},
	};

	RefCountPtr<ID3D10Blob> blob{ nullptr }, error{ nullptr };
	D3D_SHADER_MACRO defines{ nullptr, nullptr };
	HRESULT hr = D3DCompile(data, dataLen, nullptr, &defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entryInfo[stage].entry, entryInfo[stage].target,
		D3DCOMPILE_DEBUG, 0, blob.GetComRef(), error.GetComRef());
	if (error) {
		Log<LERROR>((char*)error->GetBufferPointer());
		return nullptr;
	}

	return SUCCEEDED(hr) ? new DX12ShaderStage(stage, blob) : nullptr;
}

DX12ShaderStage::DX12ShaderStage(ERHIShaderStage stage, ID3D10Blob* blob) :
	mStage(stage),
	mBlob(blob) {
}

// ==== DX12ShaderInstance ====
DX12ShaderInstance::DX12ShaderInstance(DX12Shader* shader) :
	mShader(shader) {
	// create constant buffer
	for (auto& iter : shader->mCBufferMap) {
		uint32_t slot = iter.second.index;
		uint32_t size = iter.second.size;
		if (slot >= mCBuffers.size()) {
			mCBuffers.resize(slot + 1);
		}
		mCBuffers[slot] = new DX12ConstantBuffer(size);
	}
	uint32_t max_slot = 0;
	for (auto& iter : shader->mTextureMap) {
		max_slot = std::max(max_slot, iter.second.index + 1);
	}
	mTextures.resize(max_slot);
	mSamplers.resize(max_slot);
}

void DX12ShaderInstance::SetParameter(const std::string& key, const float* value, int size) {
	auto iter = mShader->mVariableMap.find(key);
	if (iter != mShader->mVariableMap.end()) {
		CHECK(size * sizeof(float) == iter->second.size);
		mCBuffers[iter->second.index]->CopyData(value, iter->second.offset,  iter->second.size);
	}
}

void DX12ShaderInstance::SetParameter(const std::string& key, RHITexture* tex, uint32_t samplerFlag) {
	auto iter = mShader->mTextureMap.find(key);
	if (iter != mShader->mTextureMap.end()) {
		uint32_t index = iter->second.index;
		mTextures[index] = static_cast<DX12Texture*>(tex);
		if (mSamplers[index] == nullptr || mSamplers[index]->GetSamplerFlag() != samplerFlag) {
			mSamplers[index] = new DX12Sampler(samplerFlag);
		}
	}
};


std::vector<ID3D12DescriptorHeap*> DX12ShaderInstance::GetUsedHeap() const {
	std::vector<ID3D12DescriptorHeap*> usedHeap;
	auto PushIfUnique = [&usedHeap](const ID3D12DescriptorHeap* heap) {
		for (int i = 0; i < usedHeap.size(); i++) {
			if (usedHeap[i] == heap) return;
		}
		usedHeap.push_back(const_cast<ID3D12DescriptorHeap*>(heap));
	};

	for (int i = 0; i < mCBuffers.size(); i++) {
		if (mCBuffers[i]) {
			const ID3D12DescriptorHeap* heap = mCBuffers[i]->GetView()->GetHeap();
			PushIfUnique(heap);
		}
	}
	for (int i = 0; i < mTextures.size(); i++) {
		if (mTextures[i]) {
			const ID3D12DescriptorHeap* heap = mTextures[i]->GetView()->GetHeap();
			PushIfUnique(heap);
		}
	}

	for (int i = 0; i < mSamplers.size(); i++) {
		if (mSamplers[i]) {
			const ID3D12DescriptorHeap* heap = mSamplers[i]->GetView()->GetHeap();
			PushIfUnique(heap);
		}
	}
	return usedHeap;
}

std::vector<D3D12_GPU_DESCRIPTOR_HANDLE> DX12ShaderInstance::GetDescriptorTable() const {
	std::vector<D3D12_GPU_DESCRIPTOR_HANDLE> result;
	for (auto& iter : mShader->mCBufferMap) {
		DX12ConstantBuffer* buffer = mCBuffers[iter.second.index];
		if (buffer) {
			result.push_back(buffer->GetView()->GetGPUHandle());
		} else {
			result.push_back(D3D12_GPU_DESCRIPTOR_HANDLE{ 0 });
		}
	}
	for (auto& iter : mShader->mTextureMap) {
		DX12Texture* tex = mTextures[iter.second.index];
		if (tex) {
			result.push_back(tex->GetView()->GetGPUHandle());
		} else {
			result.push_back(D3D12_GPU_DESCRIPTOR_HANDLE{0});
		}
	}
	for (auto& iter : mShader->mSamplerMap) {
		DX12Sampler* sampler = mSamplers[iter.second.index];
		if (sampler) {
			result.push_back(sampler->GetView()->GetGPUHandle());
		} else {
			result.push_back(D3D12_GPU_DESCRIPTOR_HANDLE{0});
		}
	}
	return result;
}


// ==== DX12Shader ====
DX12Shader::DX12Shader() {

}

DX12Shader::~DX12Shader() {
	for (char *name : mInputNameMem) {
		delete name;
	}
	mInputNameMem.clear();
}

void DX12Shader::Reflect() {
	std::vector<DX12ShaderStage*> stages{ mStageVS , mStagePS};
	for (DX12ShaderStage* stage : stages) {
		if (stage == nullptr) {
			continue;
		}
		RefCountPtr<ID3D12ShaderReflection> reflection;
		D3DReflect(stage->GetCode().pShaderBytecode, stage->GetCode().BytecodeLength, IID_PPV_ARGS(reflection.GetComRef()));
		
		D3D12_SHADER_DESC shaderDesc;
		reflection->GetDesc(&shaderDesc);

		ReflectBoundResource(reflection, shaderDesc);

		if (stage->GetStage() == SHADER_STAGE_VERTEX) {
			ReflectInput(reflection, shaderDesc);
			ReflectConstantBuffer(reflection, shaderDesc);
		}
	}
}

void DX12Shader::ReflectInput(ID3D12ShaderReflection* reflection, const D3D12_SHADER_DESC &shaderDesc) {
	mInputELementsDesc.clear();
	for (uint32_t i = 0; i < shaderDesc.InputParameters; i++) {
		D3D12_SIGNATURE_PARAMETER_DESC paramDesc;
		reflection->GetInputParameterDesc(i, &paramDesc);
		
		D3D12_INPUT_ELEMENT_DESC inputDesc;
		char* name = new char[strlen(paramDesc.SemanticName) + 1];
		strcpy(name, paramDesc.SemanticName);
		mInputNameMem.emplace_back(name);

		inputDesc.SemanticName = name;
		inputDesc.SemanticIndex = paramDesc.SemanticIndex;
		inputDesc.InputSlot = 0;
		inputDesc.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
		inputDesc.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
		inputDesc.InstanceDataStepRate = 0;

#define DECIDE_FORMAT(_mask, _suffix, _output) {							\
		if (_mask == 1) _output = DXGI_FORMAT_R32_##_suffix;				\
		else if (_mask <= 3) _output = DXGI_FORMAT_R32G32_##_suffix;		\
		else if (_mask <= 7) _output = DXGI_FORMAT_R32G32B32_##_suffix;		\
		else if (_mask <= 15) _output = DXGI_FORMAT_R32G32B32A32_##_suffix;}

		if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) {
			DECIDE_FORMAT(paramDesc.Mask, UINT, inputDesc.Format);
		} else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) {
			DECIDE_FORMAT(paramDesc.Mask, FLOAT, inputDesc.Format);
		} else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) {
			DECIDE_FORMAT(paramDesc.Mask, SINT, inputDesc.Format);
		}
#undef DECIDE_FORMAT

		mInputELementsDesc.emplace_back(inputDesc);
	}
}

void DX12Shader::ReflectConstantBuffer(ID3D12ShaderReflection* reflection, const D3D12_SHADER_DESC &shaderDesc) {
	for (uint32_t i = 0; i < shaderDesc.ConstantBuffers; i++) {
		ID3D12ShaderReflectionConstantBuffer *cb = reflection->GetConstantBufferByIndex(i);
		D3D12_SHADER_BUFFER_DESC cbDesc;
		cb->GetDesc(&cbDesc);
		CBufferInfo &cbInfo = mCBufferMap[cbDesc.Name];
		cbInfo.size = cbDesc.Size;

		for (int vari = 0; vari < cbDesc.Variables; vari++) {
			ID3D12ShaderReflectionVariable *var = cb->GetVariableByIndex(vari);
			D3D12_SHADER_VARIABLE_DESC varDesc;
			var->GetDesc(&varDesc);

			mVariableMap[varDesc.Name] = { cbInfo.index, varDesc.StartOffset, varDesc.Size };
		}
	}
}

void DX12Shader::ReflectBoundResource(ID3D12ShaderReflection* reflection, const D3D12_SHADER_DESC &shaderDesc) {
	for (uint32_t i = 0; i < shaderDesc.BoundResources; i++) {
		D3D12_SHADER_INPUT_BIND_DESC sibDesc;
		reflection->GetResourceBindingDesc(i, &sibDesc);
		Log<LINFO>(sibDesc.Name, sibDesc.BindPoint, sibDesc.Type);

		if (sibDesc.Type == D3D_SIT_CBUFFER && mCBufferMap.count(sibDesc.Name) == 0) {
			mCBufferMap[sibDesc.Name] = {sibDesc.BindPoint, 0};
			continue;
		} 
		if (sibDesc.Type == D3D_SIT_TEXTURE) {
			mTextureMap[sibDesc.Name] = { sibDesc.BindPoint };
			continue;
		}
		if (sibDesc.Type == D3D_SIT_SAMPLER) {
			mSamplerMap[sibDesc.Name] = { sibDesc.BindPoint };
			continue;
		}
	}
}

bool DX12Shader::Complete() {
	Reflect();
	if (Validate()) {
		CreateRootSignature();
	
		return true;
	}
	return false;
}

bool DX12Shader::Validate() {
	// check sampler index equel texture index
	std::vector<int> samplerIndexs;
	std::vector<int> textureIndexs;
	for (auto& iter : mTextureMap) {
		textureIndexs.push_back(iter.second.index);
	}
	for (auto& iter : mSamplerMap) {
		samplerIndexs.push_back(iter.second.index);
	}
	std::sort(samplerIndexs.begin(), samplerIndexs.end());
	std::sort(textureIndexs.begin(), textureIndexs.end());

	if (samplerIndexs.size() != textureIndexs.size()) {
		return false;
	}
	for (int i = 0; i < samplerIndexs.size(); i++) {
		if (samplerIndexs[i] != textureIndexs[i]) {
			return false;
		}
	}

	// check input Semantic
#define CHECK_AND_APPEND_SEMNATIC(desc, name, index, sem)	\
if (strcmp(desc.SemanticName, #name) == 0 && desc.SemanticIndex == index && GetPixelSize(desc.Format) == GetSemanticSize(sem)) {	\
	mInputSemantics.push_back(sem);		\
	continue;	\
}

	for (size_t i = 0; i < mInputELementsDesc.size(); i++) {
		D3D12_INPUT_ELEMENT_DESC &desc = mInputELementsDesc[i];
		CHECK_AND_APPEND_SEMNATIC(desc, POSITION, 0, SEMANTIC_POSITION);
		CHECK_AND_APPEND_SEMNATIC(desc, NORMAL, 0, SEMANTIC_NORMAL);
		CHECK_AND_APPEND_SEMNATIC(desc, TANGENT, 0, SEMANTIC_TANGENT);
		CHECK_AND_APPEND_SEMNATIC(desc, BINORMAL, 0, SEMANTIC_BINORMAL);
		CHECK_AND_APPEND_SEMNATIC(desc, TEXCOORD, 0, SEMANTIC_UV0);
		CHECK_AND_APPEND_SEMNATIC(desc, TEXCOORD, 1, SEMANTIC_UV1);
		// not matched...
		return false;
	}

#undef CHECK_AND_APPEND_SEMNATIC
	return true;
}

D3D12_INPUT_LAYOUT_DESC DX12Shader::GetInputLayoutDesc() {
	return { mInputELementsDesc.data(), (uint32_t)mInputELementsDesc.size() };
}

ID3D12RootSignature* DX12Shader::GetIRootSignature() const {
	return mRootSignature;
}

const std::vector<ERHIInputSemantic>& DX12Shader::GetInputSemantics() const {
	return mInputSemantics;
}

void DX12Shader::CreateRootSignature() {
	CD3DX12_ROOT_PARAMETER rootParam[MAX_SIGNATURE_NUM];
	CD3DX12_DESCRIPTOR_RANGE cbvTable[MAX_SIGNATURE_NUM];
	uint32_t count = 0;
	for (auto &iter: mCBufferMap) {
		cbvTable[count].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, iter.second.index);
		rootParam[count].InitAsDescriptorTable(1, &cbvTable[count]);
		count++;
	}
	for (auto& iter : mTextureMap) {
		cbvTable[count].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, iter.second.index);
		rootParam[count].InitAsDescriptorTable(1, &cbvTable[count]);
		count++;
	}
	for (auto &iter : mSamplerMap) {
		cbvTable[count].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 1, iter.second.index);
		rootParam[count].InitAsDescriptorTable(1, &cbvTable[count]);
		count++;
	}

	CD3DX12_ROOT_SIGNATURE_DESC rootSignDesc(count, rootParam, 0, nullptr,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	RefCountPtr<ID3DBlob> serialzed, error;
	HRESULT hr = D3D12SerializeRootSignature(&rootSignDesc, D3D_ROOT_SIGNATURE_VERSION_1, serialzed.GetComRef(), error.GetComRef());
	DX12_CHECK(hr, (char*)error->GetBufferPointer());

	auto device = GDX12Device->GetIDevice();
	DX12_CHECK(device->CreateRootSignature(0, serialzed->GetBufferPointer(),
		serialzed->GetBufferSize(), IID_PPV_ARGS(mRootSignature.GetComRef())));
}


}


