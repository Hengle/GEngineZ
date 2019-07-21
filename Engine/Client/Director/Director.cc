#include "Director.h"

#include <Core/CoreHeader.h>

namespace z {
Director* GDirector = nullptr;

Director::Director() {
	InitializeSingleton<Director>(GDirector, this);
	
}

Director::~Director() {
	FinalizeSingleton<Director>(GDirector, this);
}

void Director::FrameTick() {
	// begin
	BeginFrame();

	// object tick


	// render tick
	for (auto& vp : mViewports) {
		vp->Tick();
		vp->PostTick();

	}
	for (auto& vp : mViewports) {
		vp->Render();
	}

	// end
	EndFrame();;
}

void Director::Update() {
	static uint64_t last_update_ms = ZTime::Now().TotalMs();
	uint64_t now = ZTime::Now().TotalMs();

	if (last_update_ms + mFrameInterval < now) {
		last_update_ms = now;
		FrameTick();
	}
}


void Director::BeginFrame() {

}

void Director::EndFrame() {

}


void Director::AddViewport(Viewport* vp) {
	mViewports.push_back(vp);
}

void Director::DelViewport(Viewport*) {

}


}