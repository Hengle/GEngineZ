#include "DX12Shader.h"
#include "DX12Device.h"

namespace z {

RefCountPtr<DX12Shader> DX12Shader::FromCompile(const char* data, size_t dataLen, EShaderType stype) {
	// TODO cache 
	static std::string ShaderTypeToName[SHADER_TYPE_MAX]{ "", "VS", "PS" };
	static std::string ShaderTypeToTarget[SHADER_TYPE_MAX]{ "", "vs_5_0", "ps_5_0" };

	RefCountPtr<ID3D10Blob> blob{ nullptr }, error{ nullptr };
	D3D_SHADER_MACRO defines{ nullptr, nullptr };

	HRESULT hr = D3DCompile(data, dataLen, nullptr, &defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		ShaderTypeToName[stype].c_str(),
		ShaderTypeToTarget[stype].c_str(),
		D3DCOMPILE_DEBUG, 0, blob.GetComRef(), error.GetComRef());
	if (error) {
		Log<LERROR>((char*)error->GetBufferPointer());
	}
	DX12_CHECK(hr);

	RefCountPtr<DX12Shader> shader = new DX12Shader();
	shader->mBlob = blob;
	shader->mType = stype;
	return shader;
}

// DX12VertexLayout

DX12VertexLayout::~DX12VertexLayout() {
	for (int i = 0; i < mNames.size(); i++) {
		delete[] mNames[i];
	}
	mNames.clear();
	mLayout.clear();
}

void DX12VertexLayout::PushLayout(const std::string& name, uint32_t index, EPixelFormat format, EVertexLaytoutFlag flag) {
	static std::unordered_map<EVertexLaytoutFlag, D3D12_INPUT_CLASSIFICATION> classicationMapping = {
		{VERTEX_LAYOUT_PER_INSTANCE, D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA},
		{VERTEX_LAYOUT_PER_VERTEX, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA}
	};
	char *s = new char[name.length() + 1];
	memcpy(s, name.data(), name.length());
	s[name.length()] = 0;
	mNames.push_back(s);
	D3D12_INPUT_ELEMENT_DESC desc{ s, index, PixelFomat[format].format, 0, mSize, classicationMapping[flag], 0 };
	mLayout.push_back(desc);
	mSize += PixelFomat[format].size;
}


// DX12UniformLayout
void DX12UniformLayout::PushLayout(std::string name, uint32_t registerNo, EUniformLayoutFlag flag) {
	switch (flag) {
	case EUniformLayoutFlag::UNIFORM_LAYOUT_CONSTANT_BUFFER:
		if (mCBVs.size() <= registerNo) mCBVs.resize(registerNo + 1);
		mCBVs[registerNo] = name;
	}
}


RefCountPtr<ID3D12RootSignature> DX12UniformLayout::GetRootSignature() {
	if (mRootSignature) {
		return mRootSignature;
	}
	CD3DX12_ROOT_PARAMETER rootParam;
	CD3DX12_DESCRIPTOR_RANGE rangeTable[4]; 

	const std::vector<std::string> *rangeVecs[] = { &mSRVs, &mUAVs, &mCBVs, &mSamplers};
	const static D3D12_DESCRIPTOR_RANGE_TYPE rangeType[] = {
		D3D12_DESCRIPTOR_RANGE_TYPE_SRV, D3D12_DESCRIPTOR_RANGE_TYPE_UAV,
		D3D12_DESCRIPTOR_RANGE_TYPE_CBV, D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER
	};
	
	int rangeIdx = 0;
	uint32_t totalSize = 0;

	for (int i = 0; i < 4; i++) {
		const std::vector<std::string> &vec = *(rangeVecs[i]);
		if (vec.size() > 0) {
			rangeTable[rangeIdx++].Init(rangeType[i],vec.size(), 0, 0, totalSize);
			totalSize += vec.size();
		}
	}

	rootParam.InitAsDescriptorTable(rangeIdx, rangeTable, D3D12_SHADER_VISIBILITY_ALL);

	CD3DX12_ROOT_SIGNATURE_DESC rootSignDesc(1, &rootParam, 0, nullptr,
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


