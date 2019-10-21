#pragma once
#include <Core/CoreHeader.h>
#include <Client/Entity/IEntity.h>

namespace z {

class SceneCollection;
class RenderItem;
class Camera;
class PrimitiveComp;

class Scene : public IEntity {
public:
	Scene();
	bool Load(const std::string file);

	void Tick() {}

	void CollectRender(SceneCollection*);

	Camera* GetCamera() {
		return mCamera;
	}

    IEntity* Pick(const math::Vector3F& rayStart, const math::Vector3F& rayDir);

private:
	bool LoadFromFile(const std::string file);

	bool mIsEditor;
	RefCountPtr<Camera> mCamera;

	std::vector<RefCountPtr<RenderItem>> mEditorItems;
	std::vector<RefCountPtr<IEntity>> mEntities;
};

}