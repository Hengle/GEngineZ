#include "App.h"
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
	//GDirector->SetFrameLimit(60);

	OnResize(mWidth, mHeight);

}


float App::GetFps() {
	if (GDirector) {
		return GDirector->GetCurFps();
	}
	return 0.f;
}

void App::OnUpdate() {
	GDirector->Update();
}

void App::OnResize(uint32_t width, uint32_t height) {
	// be careful, this method will called before oninit
	Log<LINFO>("App On Resized....");
	mWidth = width;
	mHeight = height;

	if (GDirector) {
		GDirector->OnResize(width, height);
	}
}



}