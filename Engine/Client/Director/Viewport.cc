#include "Viewport.h"
#include <Core/Common/Common.h>
#include <Client/Scene/Scene.h>
#include <Render/RenderScene.h>

namespace z {

Viewport::Viewport() {
	mRenderScene = new RenderScene();
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
}

}