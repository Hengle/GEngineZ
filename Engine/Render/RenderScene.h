#pragma once

#include <Client/Scene/Camera.h>
#include <Client/Director/Viewport.h>

namespace z {


struct RenderSceneData {
	Camera* camera;

	void Reset() {
		camera = nullptr;
	}

};

class RenderScene {
public:
	RenderScene();

	void Begin();
	void End();

	void Render();

	RenderSceneData& Data() {
		return mData;
	}
	
protected:
	RenderSceneData mData;
};

}