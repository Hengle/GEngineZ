#include "Win32App.h"

#include <Client/Director/Director.h>
#include <Client/Director/ViewportMain.h>
#include <RHIDX12/DX12Device.h>

namespace z {

bool Win32App::Init() {
	InitWindow();

	// device
	new DX12Device(mMainWnd, mWidth, mHeight);

	// director
	new Director();
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