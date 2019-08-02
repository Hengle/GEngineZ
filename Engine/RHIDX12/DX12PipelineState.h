#pragma once

#include "DX12Header.h"
#include "DX12Device.h"
#include "DX12Shader.h"
#include "DX12Texture.h"
#include <Core/CoreHeader.h>

namespace z {



class DX12PipelineState : public RHIPipelineState {
public:
	DX12PipelineState();

	void SetShaderVS(DX12ShaderStage* shader) {
		mShaderVS = shader;
	}

	void SetShaderPS(DX12ShaderStage* shader) {
		mShaderPS = shader;
	}

	void SetVertexLayout(DX12VertexLayout* layout) {
		mVertexLayout = layout;
	}

	void SetUniformLayout(DX12UniformLayout* layout) {
		mUniformLayout = layout;
	}

	DX12UniformLayout* GetUniformLayout() {
		return mUniformLayout.GetRef();
	}

	void SetDepthStencilFormat(DXGI_FORMAT format) {
		mDepthStencilFormat = format;
	}

	void SetRenderTargetsFormat(const std::vector<DXGI_FORMAT> &format) {
		mRenderTargetsFormat = format;
	}


	D3D12_RASTERIZER_DESC& GetRasterizerState() {
		return mRasterizerState;
	}

	D3D12_BLEND_DESC& GetBlendState() {
		return mBlendState;
	}

	D3D12_DEPTH_STENCIL_DESC& GetDepthStencilState() {
		return mDepthStencilState;
	}

	void Create();

	ID3D12PipelineState* GetIPipelineState();
	ID3D12RootSignature* GetIRootSignature();

private:
	RefCountPtr<DX12ShaderStage> mShaderVS;
	RefCountPtr<DX12ShaderStage> mShaderPS;
	RefCountPtr<DX12VertexLayout> mVertexLayout;
	RefCountPtr<DX12UniformLayout> mUniformLayout;

	std::vector<DXGI_FORMAT> mRenderTargetsFormat;
	DXGI_FORMAT mDepthStencilFormat;

	RefCountPtr<ID3D12PipelineState> mState;

	D3D12_RASTERIZER_DESC mRasterizerState;
	D3D12_BLEND_DESC mBlendState;
	D3D12_DEPTH_STENCIL_DESC mDepthStencilState;
};



}
