#pragma once

#include "Win32Window.h"
#include <Core/Common/Common.h>

namespace z {

class Win32App : public Win32Window {
public:
	bool Init();
	void Run();

protected:
	void OnResize() override {
		Log<LINFO>("resized");
	}

};


};