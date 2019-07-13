#pragma once

#include <tchar.h>
#include <windows.h>
#include <windowsx.h>

namespace z {

class Window {
public:
	Window();
	~Window();
	
	virtual void OnResize() {}

	LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	bool InitWindow();
	bool UpdateWindow();


	HWND main_wnd_;
	int width_{800};
	int height_{600};
};

extern Window* GWindow;
}