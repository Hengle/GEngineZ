#pragma once
#include <Core/CoreHeader.h>
#include "Camera.h"

namespace z {
class RenderScene;
class RenderItem;

class Scene : public RefCounter {
public:
	Scene();
	bool Load();

	void Tick() {}

	void ColloectEnv(RenderScene*);
	void CollectItems(RenderScene*);

	Camera* GetCamera() {
		return mCamera;
	}

private:
	RefCountPtr<Camera> mCamera;

	std::vector<RefCountPtr<RenderItem>> items;
};

}