#include "Director.h"
#include "Input.h"
#include <Core/CoreHeader.h>
#include <Client/Main/App.h>
#include <Client/Scene/Camera.h>
#include <Client/Scene/CameraController.h>
#include <thread>
#include <chrono> 

namespace z {
Director* GDirector = nullptr;

Director::Director() {
	InitializeSingleton<Director>(GDirector, this);
	// size will changed when on resize called later
	mRenderer = new Renderer();

	LoadScene(GApp->GetContentPath() / "Test/Scene/test.scene");
	SetCameraController(new CameraController());
}

Director::~Director() {
	FinalizeSingleton<Director>(GDirector, this);
}

void Director::SetCameraController(CameraController* controller) {
	mCameraController = controller;
	controller->SetCamera(mCurScene->GetCamera());
}

void Director::LoadScene(const std::string& scnFile) {
	mCurScene = new Scene();
	mCurScene->Load(scnFile);
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
		mFrameTime = (now - last_update_ms) / 1000.0;
		last_update_ms = now;
		FrameTick();

		mFramesForStatFps++;
		if (now > mFpsStatTime + 1000) {
			mCurFps = mFramesForStatFps * 1000.0 / (now - mFpsStatTime);
			mFpsStatTime = now;
			mFramesForStatFps = 0;
		}
	} else {
		std::this_thread::sleep_for(std::chrono::milliseconds(std::min(5ULL, last_update_ms + mFrameInterval - now)));
		std::this_thread::yield();
	}

}


void Director::BeginFrame() {

}

void Director::EndFrame() {

}








}