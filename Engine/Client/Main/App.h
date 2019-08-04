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

	void OnResize(int width, int height);


protected:
	int mWidth{ 800 };
	int mHeight{ 600 };

	FilePath mRootPath;
};


extern App* GApp;;

}