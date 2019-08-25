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
			mDS->Resize(width, height);
		} else {
			mRT = new RenderTarget(width, height, PF_R16G16B16A16);
			mDS = new DepthStencil(width, height, PF_D24S8);
		}
	};
	
	RenderTarget* Render(Renderer* r, RenderTarget* src = nullptr, RenderTarget * dst = nullptr) {
		SceneCollection* sceneCol = r->GetSceneCollection();

		RenderStageScope stageScope("Forward Main");
		RenderStage::CurStage()->SetRenderTarget(mRT, mDS);
		mRT->Clear({ 0.f, 0.f, 0.f, 1.f });
		mDS->Clear(1.f, 0);

		// opaque
		for (auto item : sceneCol->FilterItems(RENDER_SET_OPAQUE)) {
			sceneCol->RetriveSceneParams(item->Material);
			item->RetriveItemParams();
			item->Draw();
		}
	
		return mRT;
	}

private:
	RefCountPtr<RenderTarget> mRT;
	RefCountPtr<DepthStencil> mDS;
};

}