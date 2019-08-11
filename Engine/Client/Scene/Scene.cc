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

#include <Client/Component/Primitive.h>


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
		mEditorItems.push_back(item);
	}

	// load sky
	{
		Mesh *mesh = MeshGenerator::CreateSphere(800, 100, 100);
		mSkyItem = new RenderItem();
		mSkyItem->material = MaterialManager::GetMaterialInstance("SphereSky");
		mSkyItem->material->SetCullMode(RS_CULL_FRONT);
		//item->material->SetFillMode(RS_FILL_WIREFRAME);
		mSkyItem->mesh = mesh;

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
		mSkyItem->material->SetParameter("tSkyTexture", tex);
	}

	
	// load primitives
	Primitive* p = new Primitive();
	if (p->LoadFromFile(GApp->GetRootPath() / "Content/Test/Nanosuit/nanosuit.model")) {
		mPrimitives.push_back(p);
	}

	return true;
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