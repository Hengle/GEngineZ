#pragma once

#include <Core/CoreHeader.h>

namespace z {
class Scene;
class RenderScene;

class Viewport {
public:
	Viewport(uint32_t width, uint32_t height);
	virtual ~Viewport();

	void Tick();
	void PostTick();
	void Render();


private:
	Scene* mScene;
	RenderScene* mRenderScene;
};


}