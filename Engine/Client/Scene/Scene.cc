#include "Scene.h"
#include <Render/RenderScene.h>
#include <Render/RenderItem.h>
#include <RHI/RHIDevice.h>
#include <RHI/RHIResource.h>

#include <Client/Main/App.h>
#include <Client/Scene/Camera.h>
#include <Client/Entity/IEntity.h>
#include <Util/Mesh/ZMeshLoader.h>
#include <Util/Image/Image.h>
#include <Util/Mesh/MeshGenerator.h>
#include <Util/Luaconf/Luaconf.h>
#include <Client/Component/PrimitiveComp.h>
#include <Client/Component/EnvComp.h>

namespace z {


Scene::Scene() {
	mIsEditor = true;
}


bool Scene::Load(const std::string file) {
	mCamera = new Camera(math::Vector3F{ 10, 10, 10 }, math::Vector3F(0, 0, 0));

	if (mIsEditor) {
		// grid
		Mesh *mesh = MeshGenerator::CreateGrid(800, 800, 40, 40);
		RenderItem* item = new RenderItem();
		item->material = MaterialManager::GetMaterialInstance(EMPTY_MATERIAL);
		item->material->SetFillMode(RS_FILL_WIREFRAME);
		item->mesh = mesh;
		mEditorItems.push_back(item);
	}

	if (!LoadFromFile(file)) {
		Log<LERROR>("Load scene failed!!!");
		return false;
	}

	Log<LINFO>("Load scene success...");
	return true;
}


bool Scene::LoadFromFile(const std::string file) {
	namespace lc = luaconf;
	lc::Value sceneData;
	if (!lc::ParseFile(file, sceneData)) {
		return false;
	}

	lc::Value envData = sceneData["env"];

	lc::Value skyData = envData["sky"];

	// scene env
	EnvComp *env = new EnvComp();
	env->Attach(this);

	// create sky
	std::string skyShader = skyData["shader"].Get<lc::string_t>();
	std::string skyTexture = GApp->GetContentPath() / skyData["texture"].Get<lc::string_t>();

	env->SetSky(skyShader, skyTexture);

	// create models
	int modelNum = sceneData["models"].Size();
	for (int modelIdx = 0; modelIdx < modelNum; modelIdx++) {
		lc::Value modelData = sceneData["models"][modelIdx];
		RefCountPtr<IEntity> ent = new IEntity();
		std::vector<float> entPos;
		if (modelData["position"].GetValSafety(entPos)) {
			ent->SetLocalPostion({ entPos[0], entPos[1], entPos[2] });
		}


		RefCountPtr<PrimitiveComp> p = new PrimitiveComp();
		if (p->LoadFromFile(GApp->GetContentPath() / modelData["model"].Get<lc::string_t>())) {
			p->Attach(ent);
			mEntities.push_back(ent);
		}
	}

}


void Scene::ColloectEnv(RenderScene* renderScn) {
	// camera info
	renderScn->ViewMatrix = mCamera->GetCam()->GetViewMatrix();
	renderScn->ViewProjMatrix = mCamera->GetCam()->GetViewProjectMatrix();

	EnvComp *comp = GetComponent<EnvComp>();
	comp->CollectRenderItems(renderScn);
}


void Scene::CollectItems(RenderScene* renderScn) {
	// editor items
	for (RenderItem* item : mEditorItems) {
		renderScn->RenderItems.push_back(item);
	}

	// primitives
	for (IEntity *ent : mEntities) {
		std::vector<PrimitiveComp*> prims = ent->GetComponents<PrimitiveComp>();
		for (auto prim : prims) {
			prim->CollectRenderItems(renderScn);
		}
	}
}

}