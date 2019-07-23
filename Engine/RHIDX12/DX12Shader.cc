#include "DX12Shader.h"

namespace z {

RefCountPtr<DX12Shader> DX12Shader::FromCompile(const char* data, size_t dataLen, ShaderType stype) {
	// TODO cache 
	static std::string ShaderTypeToName[EShaderType_MAX]{ "", "VS", "PS" };
	static std::string ShaderTypeToTarget[EShaderType_MAX]{ "", "vs_5_0", "ps_5_0" };

	RefCountPtr<ID3D10Blob> blob, error;
	D3D_SHADER_MACRO defines{ nullptr, nullptr };

	HRESULT hr = D3DCompile(data, dataLen, nullptr, &defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		ShaderTypeToName[stype].c_str(),
		ShaderTypeToTarget[stype].c_str(),
		D3DCOMPILE_DEBUG, 0, blob.GetComRef(), error.GetComRef());
	if (error) {
		Log<LERROR>((char*)error->GetBufferPointer());
	}
	DX12_CHECK(hr);

	RefCountPtr<DX12Shader> shader;
	shader->mBlob = blob;
	shader->mType = stype;
	return shader;
}

}

