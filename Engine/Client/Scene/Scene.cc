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


namespace z {


Scene::Scene() {
	mIsEditor = true;
}


bool Scene::Load() {
	mCamera = new Camera(math::Vector3F{ 10, 10, 10 }, math::Vector3F(0, 0, 0));

	if (mIsEditor) {
		// grid
		Mesh *mesh = MeshGenerator::CreateGrid(800, 800, 40, 40);
		RenderItem* item = new RenderItem();
		item->material = MaterialManager::GetMaterialInstance("empty");
		item->material->SetFillMode(RS_FILL_WIREFRAME);
		item->mesh = mesh;
		items.push_back(item);

	}

	// load sky
	{
		Mesh *mesh = MeshGenerator::CreateSphere(800, 100, 100);
		RenderItem *item = new RenderItem();
		item->material = MaterialManager::GetMaterialInstance("SphereSky");
		item->material->SetCullMode(RS_CULL_FRONT);
		//item->material->SetFillMode(RS_FILL_WIREFRAME);
		item->mesh = mesh;
		items.push_back(item);

		Image *img = Image::Load(GApp->GetRootPath() / "Content/Engine/Texture/Sky.jpg");
		RHITextureDesc desc;
		desc.sizeX = img->GetWidth();
		desc.sizeY = img->GetHeight();
		desc.sizeZ = 1;
		desc.flags = 0;
		desc.format = img->GetFormat();
		desc.dimension = TEX_DIMENSION_2D;
		desc.numMips = 1;
		RHITexture* tex = GDevice->CreateTexture(desc, img->GetData());
		item->material->SetParameter("tSkyTexture", tex);
	}

	
	MeshHub meshhub = ZMeshLoader::Load(GApp->GetRootPath() / "Content/Test/Mesh/nanosuit.zmesh");
	for (int i = 0; i < meshhub.size(); i++) {
		RenderItem *item = new RenderItem();

		item->material = MaterialManager::GetMaterialInstance("empty");
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