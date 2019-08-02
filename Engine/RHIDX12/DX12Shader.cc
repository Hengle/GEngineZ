#include "DX12Shader.h"
#include "DX12Device.h"

namespace z {


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

	}

	return SUCCEEDED(hr) ? new DX12ShaderStage(stage, blob) : nullptr;
}


DX12ShaderStage::DX12ShaderStage(ERHIShaderStage stage, ID3D10Blob* blob) :
	mStage(stage),
	mBlob(blob) {
}


void DX12Shader::Complete() {
	Reflect();
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

		// vertex input
		if (stage->GetStage() == SHADER_STAGE_VERTEX) {
			ReflectInput(reflection, shaderDesc);
		}
		

		// constant buffers



	}
}


void DX12Shader::ReflectInput(ID3D12ShaderReflection* reflection, const D3D12_SHADER_DESC &shaderDesc) {
	for (uint32_t i = 0; i < shaderDesc.InputParameters; i++) {
		D3D12_SIGNATURE_PARAMETER_DESC paramDesc;
		reflection->GetInputParameterDesc(i, &paramDesc);
		
		D3D12_INPUT_ELEMENT_DESC inputDesc;
		inputDesc.SemanticName = paramDesc.SemanticName;
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


		std::cout << inputDesc.SemanticName << inputDesc.SemanticIndex << " "<< inputDesc.Format << std::endl;
	}
	
}

	// TODO cache 
	

	//for (uint32_t i = 0; i < shaderDesc.ConstantBuffers; i++) {
	//	ID3D12ShaderReflectionConstantBuffer *b = ref->GetConstantBufferByIndex(i);
	//	D3D12_SHADER_BUFFER_DESC desc;
	//	b->GetDesc(&desc);
	//	for (int j = 0; j < desc.Variables; j++) {
	//		ID3D12ShaderReflectionVariable *var = b->GetVariableByIndex(j);
	//		D3D12_SHADER_VARIABLE_DESC desc2;
	//		var->GetDesc(&desc2);
	//		Log<LINFO>(desc2.Name);
	//	}

	//}


	//for (uint32_t i = 0; i < shaderDesc.BoundResources; i++) {
	//	D3D12_SHADER_INPUT_BIND_DESC desc;
	//	ref->GetResourceBindingDesc(i, &desc);
	//	Log<LINFO>() << "desc";
	//}
	//std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayoutDesc;
	//for (uint32_t i = 0; i < shaderDesc.InputParameters; i++)
	//{
	//	D3D12_SIGNATURE_PARAMETER_DESC paramDesc;
	//	ref->GetInputParameterDesc(i, &paramDesc);

	//	// fill out input element desc
	//	D3D12_INPUT_ELEMENT_DESC inputDesc;
	//	inputDesc.SemanticName = paramDesc.SemanticName;
	//	inputDesc.SemanticIndex = paramDesc.SemanticIndex;
	//	inputDesc.InputSlot = 0;
	//	inputDesc.AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	//	inputDesc.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
	//	inputDesc.InstanceDataStepRate = 0;

	//	// determine DXGI format
	//	if (paramDesc.Mask == 1)
	//	{
	//		if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) inputDesc.Format = DXGI_FORMAT_R32_UINT;
	//		else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) inputDesc.Format = DXGI_FORMAT_R32_SINT;
	//		else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) inputDesc.Format = DXGI_FORMAT_R32_FLOAT;
	//	} else if (paramDesc.Mask <= 3)
	//	{
	//		if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) inputDesc.Format = DXGI_FORMAT_R32G32_UINT;
	//		else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) inputDesc.Format = DXGI_FORMAT_R32G32_SINT;
	//		else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) inputDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
	//	} else if (paramDesc.Mask <= 7)
	//	{
	//		if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) inputDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
	//		else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) inputDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
	//		else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) inputDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	//	} else if (paramDesc.Mask <= 15)
	//	{
	//		if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) inputDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
	//		else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) inputDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
	//		else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) inputDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	//	}

	//	//save element desc
	//	inputLayoutDesc.push_back(inputDesc);
	//}



// DX12VertexLayout

DX12VertexLayout::~DX12VertexLayout() {
	for (int i = 0; i < mNames.size(); i++) {
		delete[] mNames[i];
	}
	mNames.clear();
	mLayout.clear();
}

void DX12VertexLayout::PushLayout(const std::string& name, ERHIPixelFormat format, EVertexLaytoutFlag flag) {
	static std::unordered_map<EVertexLaytoutFlag, D3D12_INPUT_CLASSIFICATION> classicationMapping = {
		{VERTEX_LAYOUT_PER_INSTANCE, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA},
		{VERTEX_LAYOUT_PER_VERTEX, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA}
	};
	char *s = new char[name.length() + 1];
	memcpy(s, name.data(), name.length());
	s[name.length()] = 0; 
	mNames.push_back(s);
	D3D12_INPUT_ELEMENT_DESC desc{ s, 0, FromRHIFormat(format), 0, mSize, classicationMapping[flag], 0 };
	mLayout.push_back(desc);
	mSize += GetPixelSize(FromRHIFormat(format));
}


// DX12UniformLayout
void DX12UniformLayout::PushLayout(std::string name, uint32_t registerNo, EUniformLayoutFlag flag) {
	switch (flag) {
	case EUniformLayoutFlag::UNIFORM_LAYOUT_CONSTANT_BUFFER:
		if (mCBVs.size() <= registerNo) mCBVs.resize(registerNo + 1);
		mCBVs[registerNo] = name;
		break;
	case EUniformLayoutFlag::UNIFORM_LAYOUT_TEXTURE:
		if (mSRVs.size() <= registerNo) {
			mSRVs.resize(registerNo + 1);
			mSamplers.resize(registerNo + 1);
		}
		mSRVs[registerNo] = name;
		mSamplers[registerNo] = name;
		break;
	}
}


ID3D12RootSignature *DX12UniformLayout::GetRootSignature() {
	if (mRootSignature) {
		return mRootSignature;
	}

	CD3DX12_ROOT_PARAMETER rootParam[MAX_SIGNATURE_NUM];
	CD3DX12_DESCRIPTOR_RANGE cbvTable[MAX_SIGNATURE_NUM];
	uint32_t count = 0;
	for (int i = 0; i < mCBVs.size(); i++) {
		cbvTable[count].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, i);
		rootParam[count].InitAsDescriptorTable(1, &cbvTable[count]);
		count++;
	}
	for (int i = 0; i < mSRVs.size(); i++) {
		cbvTable[count].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, i);
		rootParam[count].InitAsDescriptorTable(1, &cbvTable[count]);
		count++;
	}
	for (int i = 0; i < mSRVs.size(); i++) {
		cbvTable[count].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 1, i);
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

	return mRootSignature;
}

}


