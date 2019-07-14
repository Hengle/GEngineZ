#include "Win32Window.h"
#include <Core/Common/Common.h>

namespace z {
Win32Window* GWindow = nullptr;

Win32Window::Win32Window() {
	InitializeSingleton<Win32Window>(GWindow, this);
}

Win32Window::~Win32Window() {
	FinalizeSingleton<Win32Window>(GWindow, this);
}

LRESULT CALLBACK
MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	return GWindow->MsgProc(hwnd, msg, wParam, lParam);
}

bool Win32Window::InitWindow() {
	HINSTANCE hInstance = ::GetModuleHandle(NULL);

	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;// redraw when h or v changed
	wc.lpfnWndProc = MainWndProc;            // window process function
	wc.cbClsExtra = 0;                      // extra memory
	wc.cbWndExtra = 0;                      // extra memory
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = _T("BasicWndClass");
	wc.hIconSm = LoadIcon(0, IDI_APPLICATION);

	if (!RegisterClassEx(&wc)) {
		Log<LFATAL>("RegisterClassEx failed.");
		return false;
	}
	RECT R{ 0, 0, mWidth, mHeight };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);

    mMainWnd = CreateWindow(
		_T("BasicWndClass"),
		_T("BasicWindow"),
		WS_OVERLAPPEDWINDOW,         // style
		CW_USEDEFAULT, CW_USEDEFAULT,// x, y
		R.right - R.left,            // w
		R.bottom - R.top,            // h
		0,                           // hwnd's parent
		0,                           // menu
		hInstance,
		0                            // WM_CREATE lparam
	);

	if (!mMainWnd) {
		Log<LFATAL>("CreateWindow failed.");
		return false;
	}

	::ShowWindow(mMainWnd, SW_SHOW);
	::UpdateWindow(mMainWnd);

	return true;
}


bool Win32Window::UpdateWindow() {
	MSG msg;
	if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (msg.message == WM_QUIT) {
			return false;
		}
	}
	return true;
}

LRESULT Win32Window::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE) {
		}
		else {
		}
		return 0;
	case WM_SIZE:
		mWidth = LOWORD(lParam);
		mHeight = HIWORD(lParam);
		if (wParam == SIZE_MINIMIZED) {
		} else if (wParam == SIZE_MAXIMIZED) {
			OnResize();
		} else if (wParam == SIZE_RESTORED) {
			OnResize();
		}
		return 0;

	case WM_ENTERSIZEMOVE:
		return 0;
	case WM_EXITSIZEMOVE:
		//OnResize();
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		//OnMouseButtonEvent(wParam, 1);
		return 0;
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		//OnMouseButtonEvent(wParam, 0);
		return 0;
	case WM_MOUSEMOVE:
		//OnMouseMoveEvent(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_MOUSEWHEEL:
		//OnMouseWheelEvent(GET_Y_LPARAM(wParam));
		return 0;
	case WM_KEYDOWN:
		//OnKeyEvent(wParam, 1);
	case WM_KEYUP:
		//OnKeyEvent(wParam, 0);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}


}