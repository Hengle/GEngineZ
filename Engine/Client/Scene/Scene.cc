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
#include <Client/Component/Primitive.h>


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

	// create sky
	Mesh* mesh = MeshGenerator::CreateSphere(800, 100, 100);
	mSkyItem = new RenderItem();
	mSkyItem->material = MaterialManager::GetMaterialInstance(skyData["shader"].Get<lc::string_t>());
	mSkyItem->material->SetCullMode(RS_CULL_FRONT);
	mSkyItem->mesh = mesh;

	Image* img = Image::Load(GApp->GetContentPath() / skyData["texture"].Get<lc::string_t>());
	RHITextureDesc desc;
	desc.sizeX = img->GetWidth();
	desc.sizeY = img->GetHeight();
	desc.sizeZ = 1;
	desc.flags = 0;
	desc.format = img->GetFormat();
	desc.dimension = TEX_DIMENSION_2D;
	desc.numMips = 1;
	RHITexture* tex = GDevice->CreateTexture(desc, img->GetData());
	mSkyItem->material->SetParameter("tSkyTexture", tex);

	// create models
	int modelNum = sceneData["models"].Size();
	for (int modelIdx = 0; modelIdx < modelNum; modelIdx++) {
		lc::Value modelData = sceneData["models"][modelIdx];
		RefCountPtr<Primitive> p = new Primitive();
		if (p->LoadFromFile(GApp->GetContentPath() / modelData["model"].Get<lc::string_t>())) {
			mPrimitives.push_back(p);
		}
	}

}


void Scene::ColloectEnv(RenderScene* renderScn) {
	// camera info
	renderScn->ViewMatrix = mCamera->GetCam()->GetViewMatrix();
	renderScn->ViewProjMatrix = mCamera->GetCam()->GetViewProjectMatrix();
}


void Scene::CollectItems(RenderScene* renderScn) {
	// sky
	renderScn->RenderItems.push_back(mSkyItem);

	// editor items
	for (RenderItem* item : mEditorItems) {
		renderScn->RenderItems.push_back(item);
	}

	// primitives
	for (Primitive *prim : mPrimitives) {
		prim->CollectRenderItems(renderScn);
	}
}

}