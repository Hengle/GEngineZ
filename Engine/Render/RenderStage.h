#pragma once 
#include <RHI/RHIDevice.h>

#include "RenderTarget.h"

namespace z {

class RenderStage {
public:
	RenderStage(const std::string& name) : 
		mName(name) {
		mCurBlendState = {
			false,
			BLEND_FACTOR_ONE, BLEND_FACTOR_ZERO, BLEND_OP_ADD,
			BLEND_FACTOR_ONE, BLEND_FACTOR_ZERO, BLEND_OP_ADD
		};
	}

	void SetRenderTarget(RenderTarget* rt, DepthStencil* ds=nullptr) {
		mCurRT = rt;
		mCurDS = ds;
		GDevice->SetOutputs({ rt->GetRHIRenderTarget() }, ds ? ds->GetRHIDepthStencil() : nullptr);
		rt->GetRHIRenderTarget()->SetBlendState(mCurBlendState);
	}

	void SetRenderRect(const RHIRenderRect &rect) {
		mCurRenderRect = rect;
		GDevice->SetRenderRect(rect);
	}

	void SetScissorRect(const RHIScissorRect &rect) {
		mCurScissorRect = rect;
		GDevice->SetScissorRect(rect);
	}

	void SetBlendState(const RHIBlendState& state) {
		mCurBlendState = state;
		if (mCurRT) {
			mCurRT->GetRHIRenderTarget()->SetBlendState(state);
		}
	}

	void CopyStage(RenderStage& other) {
		other.mCurRT = mCurRT;
		other.mCurDS = mCurDS;
		other.mCurRenderRect = mCurRenderRect;
		other.mCurScissorRect = mCurScissorRect;
		other.mCurBlendState = mCurBlendState;
	}

	void RecoverDevice() {
		GDevice->SetOutputs({ mCurRT->GetRHIRenderTarget() }, mCurDS ? mCurDS->GetRHIDepthStencil() : nullptr);
		mCurRT->GetRHIRenderTarget()->SetBlendState(mCurBlendState);
		GDevice->SetRenderRect(mCurRenderRect);
		GDevice->SetScissorRect(mCurScissorRect);
	}

	// static method
	static void BeginStage(const std::string& name);
	static void EndStage(bool recover = false);
	static RenderStage* CurStage();

private:
	static void RecoverStage();
	static std::vector<RenderStage> gStageStack;

	std::string mName;
	RefCountPtr<RenderTarget> mCurRT;
	RefCountPtr<DepthStencil> mCurDS;
	RHIBlendState mCurBlendState;
	RHIRenderRect mCurRenderRect;
	RHIScissorRect mCurScissorRect;
};

class RenderStageScope {
public:
	RenderStageScope(const std::string &name, bool recover=false):
		mRecover(recover) {
		RenderStage::BeginStage(name);
	}

	~RenderStageScope() {
		RenderStage::EndStage(mRecover);
	}

private:
	bool mRecover;


};


}