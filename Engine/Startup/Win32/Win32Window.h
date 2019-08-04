#pragma once

#include <tchar.h>
#include <windows.h>
#include <windowsx.h>

namespace z {

class Win32EventListener {

};

class Win32Window {
public:
	Win32Window(int width, int height);
	~Win32Window();
	
	virtual void OnResize(int width, int height) = 0;
	virtual void OnMouseButtonEvent(UINT_PTR, bool) = 0;
	virtual void OnMouseWheelEvent(int) = 0;
	virtual void OnMouseMoveEvent(int, int) = 0;
	virtual void OnKeyboardEvent(UINT_PTR, bool) = 0;

	LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	bool InitWindow();
	bool UpdateWindow();

	HWND mMainWnd{ nullptr };

private:
	int mWindowWidth;
	int mWindowHeight;
};

extern Win32Window* GWindow;
}