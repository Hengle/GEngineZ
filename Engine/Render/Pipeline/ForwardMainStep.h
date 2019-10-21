#pragma once
#include <Core/CoreHeader.h>

#include "RenderStep.h"
#include "../RenderTarget.h"
#include "../RenderStage.h"

namespace z {

class ForwardMainStep : public RenderStep {
public:
	void Init() override {

	}

	void Resize(uint32_t width, uint32_t height) override {
		if (mRT) {
			mRT->Resize(width, height);
		} else {
			mRT = new RenderTarget(width, height, PF_R16G16B16A16);
		}
	};
	
	RenderTarget* Render(Renderer* r, RenderTarget* src = nullptr, RenderTarget * dst = nullptr) {
		SceneCollection* sceneCol = r->GetSceneCollection();

		RenderStageScope stageScope("Forward Main");

		mRT->Clear({ 0.f, 0.f, 0.f, 1.f });
		RenderStage::CurStage()->SetRenderTarget(mRT, r->GetDepthStencil());

		// opaque
		for (auto item : sceneCol->FilterItems(RENDER_SET_OPAQUE)) {
			sceneCol->RetriveSceneParams(item->Material);
			item->Draw();
		}
	
		return mRT;
	}

private:
	RefCountPtr<RenderTarget> mRT;
};

}