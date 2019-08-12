#pragma once
#include <Core/CoreHeader.h>
#include <Client/Entity/IEntity.h>

namespace z {
class RenderScene;
class RenderItem;
class Camera;
class PrimitiveComp;

class Scene : public IEntity {
public:
	Scene();
	bool Load(const std::string file);

	void Tick() {}

	void ColloectEnv(RenderScene*);
	void CollectItems(RenderScene*);

	Camera* GetCamera() {
		return mCamera;
	}

private:
	bool LoadFromFile(const std::string file);

	bool mIsEditor;
	RefCountPtr<Camera> mCamera;

	std::vector<RefCountPtr<RenderItem>> mEditorItems;
	RefCountPtr<RenderItem> mSkyItem;
	std::vector<RefCountPtr<IEntity>> mEntities;
};

}