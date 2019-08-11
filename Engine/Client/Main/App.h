#pragma once
#include <Core/CoreHeader.h>
#include "Input.h"

namespace z {

class App {
public:
	App();
	~App();

	FilePath GetRootPath() {
		return mRootPath;
	}

	void OnInit(FilePath rootPath);
	void OnUpdate();

	void OnResize(uint32_t width, uint32_t height);

	float GetFps();


protected:
	int mWidth{ 800 };
	int mHeight{ 600 };

	FilePath mRootPath;
};


extern App* GApp;;

}