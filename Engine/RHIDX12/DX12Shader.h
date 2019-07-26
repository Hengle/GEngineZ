#pragma once

#include "DX12Header.h"
#include "DX12Resource.h"
#include "DX12Buffer.h"

namespace z {

// Shader
// VertexLayout
// UniformLayout
// ShaderHub = PS Shader + VS Shader + Vertex Layout + Uniform Layout

class DX12Shader : public RHIShader {
public:
	static RefCountPtr<DX12Shader> FromCompile(const char* data, size_t dataLen, EShaderType stype);

	EShaderType GetShaderType() {
		return mType;
	}

	D3D12_SHADER_BYTECODE GetCode() const {
		return { reinterpret_cast<BYTE*>(mBlob->GetBufferPointer()), mBlob->GetBufferSize() };
	}

	//void GetViewVS();
	//void GetViewPS();

private:
	DX12Shader() :mType(SHADER_TYPE_UNKNOWN) {}
	RefCountPtr<ID3D10Blob> mBlob{nullptr};
	EShaderType mType;
};


class DX12VertexLayout : public RHIVertexLayout {
public:
	// todo, use common type
	DX12VertexLayout() : mSize(0) {}

	void PushLayout(std::string semanticName, uint32_t semanticIndex, EPixelFormat format, EVertexLaytoutFlag flag);
	
private:
	std::vector<D3D12_INPUT_ELEMENT_DESC> mLayout;
	uint32_t mSize{ 0 };
};

class DX12UniformLayout : public RHIUniformLayout {
public:
	void PushLayout(std::string name, uint32_t registerno, EUniformLayoutFlag flag);

	RefCountPtr<ID3D12RootSignature> GetRootSignature();

private:
	std::vector<std::string> mCBVs;
	std::vector<std::string> mSRVs;
	std::vector<std::string> mUAVs;
	std::vector<std::string> mSamplers;

	RefCountPtr<ID3D12RootSignature> mRootSignature;

};


class DX12ShaderHub {
public:
	DX12ShaderHub(DX12Shader* shaderVs, DX12Shader* shaderPS, DX12VertexLayout* vertexLayout, DX12UniformLayout* uniformLayout) {
		mShaderVS = shaderVs;
		mShaderPS = shaderPS;
		mVertexLayout = vertexLayout;
		mUniformLayout = uniformLayout;
	}

private:
	RefCountPtr<DX12Shader> mShaderPS;
	RefCountPtr<DX12Shader> mShaderVS;
	RefCountPtr<DX12VertexLayout> mVertexLayout;
	RefCountPtr<DX12UniformLayout> mUniformLayout;
};

class DX12RootSignature {

};





}
