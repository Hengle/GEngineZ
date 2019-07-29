#pragma once

#include "DX12Header.h"
#include "DX12Resource.h"
#include "DX12Buffer.h"
#include <Core/CoreHeader.h>

namespace z {

// Shader(VS, PS...)
// VertexLayout
// UniformLayout
// RenderTargetDesc
// DepthStencilDesc

class DX12Shader : public RHIShader {
public:
	static DX12Shader* FromCompile(const char* data, size_t dataLen, ERHIShaderType stype);

	ERHIShaderType GetShaderType() override {
		return mType;
	}
	D3D12_SHADER_BYTECODE GetCode() const {
		return { reinterpret_cast<BYTE*>(mBlob->GetBufferPointer()), mBlob->GetBufferSize() };
	}

	//void GetViewVS();
	//void GetViewPS();

//private:
	DX12Shader() :mType(SHADER_TYPE_UNKNOWN) {}
	RefCountPtr<ID3D10Blob> mBlob{nullptr};
	ERHIShaderType mType;
};


class DX12VertexLayout : public RHIVertexLayout {
public:
	// todo, use common type
	DX12VertexLayout() : mSize(0) {}

	~DX12VertexLayout();

	void PushLayout(const std::string& semanticName, uint32_t semanticIndex, ERHIPixelFormat format, EVertexLaytoutFlag flag) override;
	
	D3D12_INPUT_LAYOUT_DESC GetDesc() {
		return { mLayout.data(), (uint32_t)mLayout.size() };
	}

private:
	std::vector<char*> mNames;
	std::vector<D3D12_INPUT_ELEMENT_DESC> mLayout;
	uint32_t mSize{ 0 };
};

class DX12UniformLayout : public RHIUniformLayout {
public:
	void PushLayout(std::string name, uint32_t registerNo, EUniformLayoutFlag flag) override;

	ID3D12RootSignature* GetRootSignature();

	size_t GetCBVNum() {
		return mCBVs.size();
	}

	size_t GetSRVNum() {
		return mSRVs.size();
	}

	size_t GetSamplerNum() {
		return mSamplers.size();
	}

	size_t GetCBVStart() {
		return 0;
	}

	size_t GetSRVStart() {
		return GetCBVNum();
	}

	size_t GetSamplerStart() {
		return GetSRVStart() + GetSRVNum();
	}

private:
	std::vector<std::string> mCBVs;
	std::vector<std::string> mSRVs;
	std::vector<std::string> mUAVs;
	std::vector<std::string> mSamplers;

	// CBV...|SRV...|Sampler...|UAV

	RefCountPtr<ID3D12RootSignature> mRootSignature;

};


class DX12BlendStateDesc {

};





}
