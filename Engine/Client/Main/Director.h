#pragma once

#include <vector>
#include <Client/Scene/Scene.h>
#include <Core/CoreHeader.h>

#include "Viewport.h"

namespace z {

class Director {
public:
	Director();
	~Director();
	
	void Update();
	
	void FrameTick();
	void BeginFrame();
	void EndFrame();

	void SetFrameLimit(int limit) {
		mFrameInterval = int(1000.0f / limit);
	}


	Viewport* GetViewport() {
		return mViewport;
	}



private:
	uint64_t mFrameInterval{ 0 };
	Viewport* mViewport;
	
};

extern Director* GDirector;


}

