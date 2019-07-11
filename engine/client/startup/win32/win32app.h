#pragma once

#include "window.h"
#include <core/common/common.h>

namespace z {

class Win32App : public Window {
public:
	bool Init();
	void Run();

protected:
	void OnResize() override {
		Log<LINFO>("resized");
	}

};


};