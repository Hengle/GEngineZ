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

	FilePath GetContentPath() {
		return mRootPath / "Content";
	}

	void OnInit(FilePath rootPath);
	void OnUpdate();

	void OnResize(uint32_t width, uint32_t height);

	float GetFps();

	// Editor GUI
	bool IsEditorUIEnable() {
		return mEnableEditorUI;
	}

	void SetEditorUIEnable(bool enable) {
		mEnableEditorUI = enable;
	}

	virtual void InitEditorUI() = 0;
	virtual void ReleaseEditorUI() = 0;
	virtual void EditorUINewFrame() = 0;


protected:
	int mWidth{ 800 };
	int mHeight{ 600 };

	FilePath mRootPath;
	bool mEnableEditorUI;
};


extern App* GApp;;

}