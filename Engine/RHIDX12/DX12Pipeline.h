#pragma once

#include "DX12Header.h"
#include "DX12Device.h"
#include "DX12Shader.h"
#include <Core/CoreHeader.h>

namespace z {

class DX12PipelineState {
public:
	// todo, cache
	static DX12PipelineState* GetCache(DX12Shader*, DX12Shader*, DX12VertexLayout*);

	void DoSetPipeline();
private:

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

	DX12RootSignature *mRootSignature;
	int mRenderTargetNum;
	bool mHasDepthStencil;
};



}
