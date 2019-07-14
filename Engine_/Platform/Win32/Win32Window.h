#pragma once

#include <tchar.h>
#include <windows.h>
#include <windowsx.h>

namespace z {

class Win32Window {
public:
	Win32Window();
	~Win32Window();
	
	virtual void OnResize() {}

	LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	bool InitWindow();
	bool UpdateWindow();


	HWND mMainWnd;
	int mWidth{800};
	int mHeight{600};
};

extern Win32Window* GWindow;
}