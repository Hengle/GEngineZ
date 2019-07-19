#pragma once

namespace z {
class Scene;
class RenderScene;

class Viewport {
public:
	Viewport();
	virtual ~Viewport();

	void Tick();
	void PostTick();
	void Render();


private:
	Scene* mScene;
	RenderScene* mRenderScene;
};


}