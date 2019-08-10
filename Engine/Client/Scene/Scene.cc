#include "Scene.h"
#include <Render/RenderScene.h>
#include <Render/RenderItem.h>

#include <Client/Main/App.h>
#include <Util/Mesh/ZMeshLoader.h>
#include <Util/Image/Image.h>

namespace z {


Scene::Scene() {}


bool Scene::Load() {
	mCamera = new Camera(math::Vector3F{ 10, 10, 10 }, math::Vector3F(0, 0, 0));
	
	MeshHub meshhub = ZMeshLoader::Load(GApp->GetRootPath() / "Content/Test/Mesh/nanosuit.zmesh");
	for (int i = 0; i < meshhub.size(); i++) {
		RenderItem *item = new RenderItem();

		item->material = MaterialManager::GetMaterialInstance("test");
		item->mesh = meshhub[i];
		items.push_back(item);
	}

	// texture

	return true;
}


void Scene::ColloectEnv(RenderScene* renderScn) {
	// camera info
	renderScn->ViewMatrix = mCamera->GetCam()->GetViewMatrix();
	renderScn->ViewProjMatrix = mCamera->GetCam()->GetViewProjectMatrix();
}


void Scene::CollectItems(RenderScene* renderScn) {
	for (auto item : items) {
		renderScn->RenderItems.push_back(item);
	}

}

}