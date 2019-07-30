#pragma once

#include <vector>
#include <Client/Scene/Scene.h>
#include <Core/CoreHeader.h>

#include "Viewport.h"

namespace z {

class Director {
public:
	Director(FilePath rootPath);
	~Director();
	
	void Update();
	
	void FrameTick();
	void BeginFrame();
	void EndFrame();

	void SetFrameLimit(int limit) {
		mFrameInterval = int(1000.0f / limit);
	}

	void AddViewport(Viewport*);
	void DelViewport(Viewport*);

	FilePath GetRootPath() {
		return mRootPath;
	}

	void LoadShaders();

private:
	uint64_t mFrameInterval{ 0 };
	std::vector<Viewport*> mViewports;

	FilePath mRootPath;
	
};

extern Director* GDirector;


}

