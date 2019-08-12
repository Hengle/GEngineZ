#include "Renderer.h"
#include <Client/Scene/Scene.h>
#include <Client/Main/App.h>
#include <Client/Main/Director.h>
#include "RenderScene.h"

namespace z {

Renderer::Renderer() :
	mViewportWidth(0),
	mViewportHeight(0) {
	MaterialManager::LoadShaders(GApp->GetRootPath() / "Content/Engine/Shader/hlsl");
	mRenderScene = new RenderScene();
}

Renderer::~Renderer() {

}

void Renderer::Resize(uint32_t width, uint32_t height) {
	mViewportWidth = width;
	mViewportHeight = height;
	if (!mRHIViewport) {
		mRHIViewport = GDevice->CreateViewport(width, height, PF_R8G8B8A8);
	} else {
		mRHIViewport->Resize(width, height);
	}

	mDepthStencil = GDevice->CreateDepthStencil(width, height, PF_D24S8);

}

void Renderer::Tick() {
	if (!mRHIViewport) {
		return;
	}

	// reset render scene
	mRenderScene->Reset();

	Scene* scn = GDirector->GetCurScene();
	if (scn) {
		scn->ColloectEnv(mRenderScene);
		scn->CollectItems(mRenderScene);
	}

}

void Renderer::Render() {
	if (!mRHIViewport) {
		return;
	}

	mRHIViewport->BeginDraw(RHIClearValue(1.0, 1.0, 1.0, 1.0));



	// === Render Scene ===
	mDepthStencil->Clear(RHIClearValue(1.0, 0));
	GDevice->SetOutputs({ mRHIViewport->GetBackBuffer() }, mDepthStencil);

	// render oqaque
	// render sky

	// just simple render everything now	
	for (auto item : mRenderScene->RenderItems) {
		CollectMaterialParametes(item);
		item->material->DrawIndexed(item->mesh->mVBuffer, item->mesh->mIBuffer);
	}

	// === Post Process ===

	mRHIViewport->EndDraw();

	// free resoruce and so on..
	GDevice->EndDraw();
}


void Renderer::CollectMaterialParametes(RenderItem* item) {
	math::Matrix4 world = math::Matrix4::Identity;
	// per frame
	item->material->SetParameter("ViewProj", (const float*)& mRenderScene->ViewProjMatrix, 16);

	// per object
	item->material->SetParameter("World", (const float*)& item->WorldMatrix, 16);
}

}