#include "App.h"
#include "Viewport.h"
#include "Director.h"

namespace z {

App* GApp = nullptr;

App::App() {
	InitializeSingleton<App>(GApp, this);
}

App::~App() {
	FinalizeSingleton<App>(GApp, this);
}

void App::OnInit(FilePath rootPath) {
	Log<LINFO>("App On Inited....");
	mRootPath = rootPath;
	new Input();
	new Director();
	GDirector->SetFrameLimit(30);

	OnResize(mWidth, mHeight);

}

void App::OnUpdate() {
	GDirector->Update();
}

void App::OnResize(int width, int height) {
	// be careful, this method will called before oninit
	Log<LINFO>("App On Resized....");
	mWidth = width;
	mHeight = height;

	if (GDirector) {
		Viewport* vp = GDirector->GetViewport();
		vp->Resize(width, height);
	}
}



}