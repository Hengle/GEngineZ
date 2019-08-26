#include "Renderer.h"
#include <Client/Scene/Scene.h>
#include <Client/Main/App.h>
#include <Client/Main/Director.h>
#include <RHI/RHIDevice.h>

#include "RenderOption.h"
#include "RenderStage.h"
#include "SceneCollection.h"

#include "Pipeline/IMGuiStep.h"
#include "Pipeline/ForwardMainStep.h"
#include "Pipeline/HDRStep.h"

namespace z {

RenderOptions GRenderOptions;

Renderer::Renderer() :
	mViewportWidth(0),
	mViewportHeight(0) {
	MaterialManager::LoadShaders(GApp->GetRootPath() / "Shader");
	mSceneCol = new SceneCollection();

	// render steps
	mRenderSteps[RENDER_STEP_IMGUI] = new IMGuiStep();
	mRenderSteps[RENDER_STEP_FORWARD_MAIN] = new ForwardMainStep();
	mRenderSteps[RENDER_STEP_HDR] = new HDRStep();

	// init all render steps
	for (auto& iter : mRenderSteps) {
		iter.second->Init();
	}
}

Renderer::~Renderer() {

}


SceneCollection* Renderer::GetSceneCollection() {
	return mSceneCol;
}

void Renderer::Resize(uint32_t width, uint32_t height) {
	mViewportWidth = width;
	mViewportHeight = height;
	if (!mRHIViewport) {
		mRHIViewport = GDevice->CreateViewport(width, height, PF_R8G8B8A8);
	} else {
		if (mBackRT)
			mBackRT->Release();
		mRHIViewport->Resize(width, height);
	}

	for (auto& iter : mRenderSteps) {
		iter.second->Resize(width, height);
	}

}

void Renderer::Tick() {
	if (!mRHIViewport) {
		return;
	}


	mSceneCol->Reset();
	Scene* scn = GDirector->GetCurScene();
	if (scn) {	
		scn->CollectRender(mSceneCol);;
	}

}

void Renderer::Render() {
	if (!mRHIViewport) {
		return;
	}


	mRHIViewport->BeginDraw();
	mBackRT = new RenderTarget(mRHIViewport->GetBackBuffer());


	RenderStage::BeginStage("Main Stage");

	RenderStage::CurStage()->SetRenderTarget(mBackRT);
	RenderStage::CurStage()->SetRenderRect({ 0.f, 0.f, (float)mViewportWidth, (float)mViewportHeight, 0.f, 1.f });
	RenderStage::CurStage()->SetScissorRect({ 0, 0, (int)mViewportWidth, (int)mViewportHeight });

	mBackRT->Clear({ 0.0, 0.0, 0.0, 1.0 });

	RenderTarget* curRT;

	// Scene Items
	curRT = mRenderSteps[RENDER_STEP_FORWARD_MAIN]->Render(this);
	// Tonemapping
	curRT = mRenderSteps[RENDER_STEP_HDR]->Render(this, curRT, mBackRT);

	DepthStencil *ds = ((ForwardMainStep*)mRenderSteps[RENDER_STEP_FORWARD_MAIN].GetRef())->GetDepthStencil();
	RenderStage::CurStage()->SetRenderTarget(mBackRT, ds);
	// Editor Items
	for (auto item : mSceneCol->FilterItems(RENDER_SET_EDITOR)) {
		mSceneCol->RetriveSceneParams(item->Material);
		item->Draw();
	}

	// === Post Process ===


	// === UI ===
	mRenderSteps[RENDER_STEP_IMGUI]->Render(this);


	RenderStage::EndStage();
	CHECK(RenderStage::CurStage() == nullptr);
	mRHIViewport->EndDraw();



	// free resoruce and so on..
	GDevice->EndDraw();
}


void RenderItems(ERenderSet set) {

}


}