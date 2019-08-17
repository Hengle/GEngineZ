#include "Win32App.h"
#include "Win32IMGuiImpl.h"

#include <Client/Main/Input.h>
#include <RHIDX12/DX12Device.h>


namespace z {

bool Win32App::Init() {
	InitWindow();

	// device
	new DX12Device(mMainWnd);


	// Get Root Path
	char exePath[MAX_PATH];
	ZeroMemory(exePath, MAX_PATH);
	::GetModuleFileName(NULL, exePath, MAX_PATH);
	int blen = strlen("Binary");
	int begin = strlen(exePath) - blen;
	for (; begin >= 0; begin--) {
		if (strncmp(exePath + begin, "Binary", blen) == 0)
			break;
	}
	exePath[begin] = 0;

	// create director
	App::OnInit(exePath);


	return true;
}

void Win32App::Run() {
	while (UpdateWindow()) {
		App::OnUpdate();
	
		float fps = App::GetFps();

		std::string fpsStr = std::to_string(fps);
		fpsStr = fpsStr.substr(0, 5);
		
		std::string windowText = "GameZ FPS: " + fpsStr;

		SetWindowText(mMainWnd, windowText.c_str());
	}


}

void Win32App::OnResize(int width, int height) {
	App::OnResize(width, height);
}

void Win32App::OnMouseButtonEvent(UINT_PTR key, bool press) {
	if (GInput == nullptr) {
		return;
	}

	// windows mouse event keycode is all current pressed mouse key
	if (press) {
		std::vector<EInput> keys = mInputHandler.OnMouseDown(key);
		for (EInput& key : keys) {
			GInput->OnMouseKey(key, EI_ACT_DOWN);
		}
	} else {
		std::vector<EInput> keys = mInputHandler.OnMouseUp(key);
		for (EInput& key : keys) {
			GInput->OnMouseKey(key, EI_ACT_UP);
		}
	}
	
}

void Win32App::OnMouseWheelEvent(int wheel) {
	if (GInput) {
		GInput->OnMouseWheel((float)wheel);
	}
}

void Win32App::OnMouseMoveEvent(int x, int y) {
	if (GInput) {
		GInput->OnMouseMove((float)x, (float(y)));
	}
}


void Win32App::OnKeyboardEvent(UINT_PTR key, bool press) {
	// handle ime
	if (key == VK_PROCESSKEY) {
		key = VK_SHIFT;
	}


	EInput ikey = mInputHandler.EncodeKey(key);

	if (ikey > 0 && GInput) {
		GInput->OnKeyboard(ikey, press ? EI_ACT_DOWN : EI_ACT_UP);
	}
};


void Win32App::InitEditorUI() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); 
	(void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(mMainWnd);
}

void Win32App::ReleaseEditorUI() {
	ImGui_ImplWin32_Shutdown();
}

void Win32App::EditorUINewFrame() {
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	bool open = true;
	ImGui::ShowDemoWindow(&open);
}



}