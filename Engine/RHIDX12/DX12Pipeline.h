#pragma once

#include "DX12Header.h"

namespace z {



class DX12Pipeline : public RHIPipeline {
public:
	DX12Pipeline();
	void SetRenderTargets(int n);
	void SetDepthStencil();
	void SetShaderVS();
	void SetShaderPS();

	void Apply();
	void Create();

private:
	RefCountPtr<ID3D12PipelineState> mPso;
	int mRenderTargetNum;
	bool mHasDepthStencil;
};

}
