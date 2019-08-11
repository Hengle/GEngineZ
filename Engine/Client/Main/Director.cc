#include "Director.h"
#include "Input.h"
#include <Core/CoreHeader.h>
#include <Client/Scene/Camera.h>
#include <Client/Scene/CameraController.h>

namespace z {
Director* GDirector = nullptr;

Director::Director() {
	InitializeSingleton<Director>(GDirector, this);
	// size will changed when on resize called later
	mRenderer = new Renderer();
	LoadScene(new Scene());

	mCameraController = new CameraController();
	mCameraController->SetCamera(mCurScene->GetCamera());



}

Director::~Director() {
	FinalizeSingleton<Director>(GDirector, this);
}

void Director::FrameTick() {
	// begin
	BeginFrame();
	// handle input
	GInput->Dispatch();
	// object tick
	mCameraController->Apply();
	mCurScene->Tick();

	// render tick
	mRenderer->Tick();
	mRenderer->Render();

	// end
	EndFrame();;
}

void Director::Update() {
	static uint64_t last_update_ms = ZTime::Now().TotalMs();
	uint64_t now = ZTime::Now().TotalMs();

	if (last_update_ms + mFrameInterval < now) {
		last_update_ms = now;
		FrameTick();

		mFramesForStatFps++;
		if (now > mFpsStatTime + 1000) {
			mCurFps = mFramesForStatFps * 1000.0 / (now - mFpsStatTime);
			mFpsStatTime = now;
			mFramesForStatFps = 0;
		}


	}
}


void Director::BeginFrame() {

}

void Director::EndFrame() {

}




}