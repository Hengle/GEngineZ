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


class DX12ShaderStage : public RHIShaderStage {
public:
	static DX12ShaderStage* FromCompile(const char* data, size_t datalen, ERHIShaderStage stage);
	
	D3D12_SHADER_BYTECODE GetCode() const {
		return { reinterpret_cast<BYTE*>(mBlob->GetBufferPointer()), mBlob->GetBufferSize() };
	}

	ERHIShaderStage GetStage() const override {
		return mStage;
	}


private:
	DX12ShaderStage(ERHIShaderStage stage, ID3D10Blob* blob);
	RefCountPtr<ID3D10Blob> mBlob{ nullptr };
	ERHIShaderStage mStage;
};

class DX12Shader : public RHIShader {
public:
	DX12Shader() {}

	void CombineStage(RHIShaderStage* rhiStage) override {
		DX12ShaderStage* stage = static_cast<DX12ShaderStage*>(rhiStage);
		if (SHADER_STAGE_VERTEX == stage->GetStage()) {
			mStageVS = stage;
		} else if (SHADER_STAGE_PIXEL == stage->GetStage()) {
			mStagePS = stage;
		}
	}

	template<typename targetStage>
	DX12ShaderStage* GetStage() {
		if (constexpr SHADER_STAGE_VERTEX == targetStage) {
			return mStageVS;
		} 
		if (constexpr SHADER_STAGE_PIXEL == targetStage) {
			return mStagePS;
		}
	}

	void Complete() override;

private:
	void Reflect();
	void ReflectInput(ID3D12ShaderReflection*, const D3D12_SHADER_DESC&);
	void ReflectConstantBuffer(ID3D12ShaderReflection*, const D3D12_SHADER_DESC&);
	void ReflectBoundResource(ID3D12ShaderReflection*, const D3D12_SHADER_DESC&);

	RefCountPtr<DX12ShaderStage> mStageVS;
	RefCountPtr<DX12ShaderStage> mStagePS;

	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputDescs;
	std::unordered_map<std::string, std::vector<D3D12_SHADER_BUFFER_DESC>> mCBuffersDesc;
	std::unordered_map<std::string, size_t> mCBuffersSize;

};


class DX12VertexLayout : public RHIVertexLayout {
public:
	// todo, use common type
	DX12VertexLayout() : mSize(0) {}

	~DX12VertexLayout();

	void PushLayout(const std::string& semanticName, ERHIPixelFormat format, EVertexLaytoutFlag flag) override;
	
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
