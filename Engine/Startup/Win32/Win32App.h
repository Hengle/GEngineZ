#pragma once

#include "Win32Window.h"
#include "Win32Input.h"
#include <Core/CoreHeader.h>
#include <Client/Main/App.h>

namespace z {

class Win32App : public App, public Win32Window {
public:
	Win32App() : Win32Window(1024, 768) {}
	bool Init();
	void Run();

protected:
	Win32InputHandler mInputHandler;

	void OnResize(int width, int height) override;
	void OnMouseButtonEvent(UINT_PTR, bool) override;
	void OnMouseWheelEvent(int) override;
	void OnMouseMoveEvent(int, int) override;
	void OnKeyboardEvent(UINT_PTR, bool) override;


};


};