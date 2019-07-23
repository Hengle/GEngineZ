#include "Viewport.h"
#include <Core/CoreHeader.h>
#include <Client/Scene/Scene.h>
#include <Render/RenderScene.h>

#include <RHIDX12/DX12Viewport.h>
#include <RHIDX12/DX12Shader.h>

namespace z {

Viewport::Viewport(uint32_t width, uint32_t height) {
	mRenderScene = new RenderScene();

	view = new DX12Viewport(width, height);
	view->Init();

}

Viewport::~Viewport() {
	SAFE_DELETE(mRenderScene);
 }

void Viewport::Tick() {
	// reset render scene
	mRenderScene->Begin();
	
	// init render scene, get view

	// culling
}

void Viewport::PostTick() {
	// collect here
	mScene->ColloectEnv(mRenderScene);
}

void Viewport::Render() {
	// render

	mRenderScene->Render();

	view->BeginDraw();
	view->EndDraw();
}

}