#pragma once 
#include <RHI/RHIDevice.h>

#include "RenderTarget.h"

namespace z {

class RenderStage {
	enum DirtyFlag {
		DIRTY_RT = 0x01,
		DIRTY_RRECT = 0x02,
		DIRTY_SRECT = 0x04,
		DIRTY_BLEND = 0x08,

		DIRTY_ALL = 0x0F
	};

public:
	RenderStage(const std::string& name) : 
		mName(name),
		mDirtyFlag(DIRTY_ALL) {
		mCurBlendState = {
			false,
			BLEND_FACTOR_ONE, BLEND_FACTOR_ZERO, BLEND_OP_ADD,
			BLEND_FACTOR_ONE, BLEND_FACTOR_ZERO, BLEND_OP_ADD
		};
	}

	void SetRenderTarget(RenderTarget* rt, DepthStencil* ds=nullptr) {
		mCurRT = rt;
		mCurDS = ds;
		mDirtyFlag |= DIRTY_RT;
	}

	void SetRenderRect(const RHIRenderRect &rect) {
		mCurRenderRect = rect;
		mDirtyFlag |= DIRTY_RRECT;
	}

	void SetScissorRect(const RHIScissorRect &rect) {
		mCurScissorRect = rect;
		mDirtyFlag |= DIRTY_SRECT;
	}

	void SetBlendState(const RHIBlendState& state) {
		mCurBlendState = state;
		mDirtyFlag |= DIRTY_BLEND;
	}

	void CopyToStage(RenderStage& other) {
		other.mCurRT = mCurRT;
		other.mCurDS = mCurDS;
		other.mCurRenderRect = mCurRenderRect;
		other.mCurScissorRect = mCurScissorRect;
		other.mCurBlendState = mCurBlendState;
	}

	void RecoverDevice() {
		if (mDirtyFlag & DIRTY_RT) {
			GDevice->SetOutputs({ mCurRT->GetRHIRenderTarget() }, mCurDS ? mCurDS->GetRHIDepthStencil() : nullptr);
		}
		if (mDirtyFlag & DIRTY_BLEND) {
			mCurRT->GetRHIRenderTarget()->SetBlendState(mCurBlendState);
		}
		if (mDirtyFlag & DIRTY_RRECT) {
			GDevice->SetRenderRect(mCurRenderRect);
		}
		if (mDirtyFlag & DIRTY_SRECT) {
			GDevice->SetScissorRect(mCurScissorRect);
		}
		mDirtyFlag = 0;
	}


	// static method
	static void BeginStage(const std::string& name);
	static void EndStage(bool recover = false);
	static RenderStage* CurStage();
	static void Apply();

private:
	static std::vector<RenderStage> gStageStack;

	std::string mName;

	uint32_t mDirtyFlag;
	RefCountPtr<RenderTarget> mCurRT;
	RefCountPtr<DepthStencil> mCurDS;
	RHIBlendState mCurBlendState;
	RHIRenderRect mCurRenderRect;
	RHIScissorRect mCurScissorRect;
};

class RenderStageScope {
public:
	RenderStageScope(const std::string &name) {
		RenderStage::BeginStage(name);
	}

	~RenderStageScope() {
		RenderStage::EndStage();
	}


};


}