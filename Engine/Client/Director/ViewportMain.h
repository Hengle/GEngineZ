#pragma once

#include "Viewport.h"
#include "AppWindow.h"

namespace z {

class ViewportMain : public Viewport {

public:
	ViewportMain(AppWindow* win);

private:
	AppWindow* mWin;
};



}