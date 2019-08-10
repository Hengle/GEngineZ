#pragma once
#include <Client/Scene/Scene.h>
#include <Core/CoreHeader.h>
#include <Render/Renderer.h>


namespace z {

class Director {
public:
	Director();
	~Director();
	
	void Update();
	
	void FrameTick();
	void BeginFrame();
	void EndFrame();

	void SetFrameLimit(int limit) {
		mFrameInterval = int(1000.0f / limit);
	}


	void LoadScene(Scene* scn) {
		mCurScene = scn;
		scn->Load();
	}

	Scene* GetCurScene() {
		return mCurScene;
	}

	Renderer* GetRenderer() {
		return mRenderer;
	}

	void OnResize(uint32_t width, uint32_t height) {
		if (mRenderer) {
			mRenderer->Resize(width, height);
		}
		if (mCurScene) {
			mCurScene->GetCamera()->SetAspect(float(width) / height);
		}
	}


private:
	uint64_t mFrameInterval{ 0 };
	
	RefCountPtr<Scene> mCurScene;
	RefCountPtr<Renderer> mRenderer;
	
};

extern Director* GDirector;


}

