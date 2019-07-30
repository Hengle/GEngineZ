#include "Win32App.h"

#include <Client/Director/Director.h>
#include <Client/Director/ViewportMain.h>
#include <RHIDX12/DX12Device.h>

#include <string.h>

namespace z {

bool Win32App::Init() {
	InitWindow();

	// device
	new DX12Device(mMainWnd);

	// director

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
	new Director(exePath);
	GDirector->SetFrameLimit(30);

	// main view
	ViewportMain* main = new ViewportMain(this);
	GDirector->AddViewport(main);

	return true;
}

void Win32App::Run() {
	while (UpdateWindow()) {
		GDirector->Update();
	
	
	}
}


}