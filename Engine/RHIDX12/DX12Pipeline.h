#pragma once

#include "DX12Header.h"
#include "DX12Device.h"
#include "DX12Shader.h"
#include "DX12Texture.h"
#include <Core/CoreHeader.h>

namespace z {


class DX12PipelineState : public RHIPipelineState {
public:
	void SetShaderVS(DX12Shader* shader) {
		mShaderVS = shader;
	}

	void SetShaderPS(DX12Shader *shader) {
		mShaderPS = shader;
	}

	void SetVertexLayout(DX12VertexLayout *layout) {
		mVertexDesc = layout;
	}

	void SetUniformLayout(DX12UniformLayout *layout) {
		mUniformDesc = layout;
	}

	void Compile();

private:
	RefCountPtr<DX12Shader> mShaderVS;
	RefCountPtr<DX12Shader> mShaderPS;
	RefCountPtr<DX12VertexLayout> mVertexDesc;
	RefCountPtr<DX12UniformLayout> mUniformDesc;

	std::vector<RefCountPtr<DX12Texture2D>> mRenderTargets;
	RefCountPtr<DX12Texture2D> mDepthStencil;

	RefCountPtr<ID3D12PipelineState> mStateCache;
};


class DX12Pipeline : public RHIPipeline {
public:
	DX12Pipeline() {}

	void SetRenderTarget();
	void SetDepthStencil();
	void SetShaderVS();
	void SetShaderPS();
	void SetInput();

	void Apply();

private:
	RefCountPtr<DX12Shader> mShaderVS;
	RefCountPtr<DX12Shader> mShaderPS;
	RefCountPtr<DX12VertexLayout> mVertexLayout;

	int mRenderTargetNum;
	bool mHasDepthStencil;
};



}
