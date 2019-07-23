#pragma once

#include "DX12Header.h"
#include "DX12Resource.h"

namespace z {


class DX12Shader : public RHIShader {
public:
	static RefCountPtr<DX12Shader> FromCompile(const char* data, size_t dataLen, ShaderType stype);

	ShaderType GetShaderType() {
		return mType;
	}

	//void GetViewVS();
	//void GetViewPS();

private:
	RefCountPtr<ID3D10Blob> mBlob;
	ShaderType mType;

};

}
