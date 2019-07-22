#pragma once

#include "Win32Window.h"
#include <Core/CoreHeader.h>
#include <Client/Director/AppWindow.h>

namespace z {

class Win32App : public AppWindow, public Win32Window {
public:
	bool Init();
	void Run();

protected:
	void OnResize() override {
		Log<LINFO>("resized");
	}

};


};