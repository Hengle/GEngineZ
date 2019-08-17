#include "Win32Window.h"
#include <Core/CoreHeader.h>

#include <imgui/imgui.h>
#include "imgui_impl_win32.h"

#include "imm.h"
#pragma comment(lib, "imm32.lib")

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace z {
Win32Window* GWindow = nullptr;

Win32Window::Win32Window(int width, int height) :
	mWindowWidth(width), 
	mWindowHeight(height) {
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
	RECT R{ 0, 0, mWindowWidth, mWindowHeight };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);

    mMainWnd = CreateWindow(
		_T("BasicWndClass"),
		_T("GameZ"),
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

	// imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(mMainWnd);

	return true;
}


bool Win32Window::UpdateWindow() {
	MSG msg = { 0 };
	while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if (msg.message == WM_QUIT) {
			return false;
		}
	}

	ImGui_ImplWin32_NewFrame();
	return true;
}

LRESULT Win32Window::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
		return true;
	switch (msg) {
	case WM_ACTIVATE:
		if (LOWORD(wParam) == WA_INACTIVE) {
		}
		else {
		}
		return 0;
	case WM_SIZE:
		mWindowWidth = LOWORD(lParam);
		mWindowHeight = HIWORD(lParam);
		if (wParam == SIZE_MINIMIZED) {
		} else if (wParam == SIZE_MAXIMIZED) {
			OnResize(mWindowWidth, mWindowHeight);
		} else if (wParam == SIZE_RESTORED) {
			OnResize(mWindowWidth, mWindowHeight);
		}
		return 0;

	case WM_ENTERSIZEMOVE:
		// grab resize bar..
		return 0;
	case WM_EXITSIZEMOVE:
		// grab resize bar..
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		OnMouseButtonEvent(wParam, true);
		return 0;
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		OnMouseButtonEvent(wParam, false);
		return 0;
	case WM_MOUSEMOVE:
		OnMouseMoveEvent(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
	case WM_MOUSEWHEEL:
		OnMouseWheelEvent(GET_Y_LPARAM(wParam));
		return 0;
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
		OnKeyboardEvent(wParam, true);
		return 0;
	case WM_SYSKEYUP:
	case WM_KEYUP:
		OnKeyboardEvent(wParam, false);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}


}