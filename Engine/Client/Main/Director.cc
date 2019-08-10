#include "Director.h"
#include "Input.h"
#include <Core/CoreHeader.h>

namespace z {
Director* GDirector = nullptr;

Director::Director() {
	InitializeSingleton<Director>(GDirector, this);
	// size will changed when on resize called later
	mViewport = new Viewport(1, 1);
	
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

	// render tick
	mViewport->Tick();
	mViewport->Render();

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




}