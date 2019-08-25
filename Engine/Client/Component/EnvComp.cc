#include "EnvComp.h"
#include <Render/RenderItem.h>
#include <RHI/RHIResource.h>
#include <Render/SceneCollection.h>
#include <Util/Mesh/MeshGenerator.h>
#include <Util/Image/Image.h>
#include <RHI/RHIDevice.h>

namespace z {

EnvComp::EnvComp() {
	mSunColor = { 1.0f, 1.0f, 1.0f };
	mAmbientColor = { 0.3f, 0.3f, 0.3f };
	mSunRotater = { 0.0f, 0.0f, 0.0f };
}


EnvComp::~EnvComp() {

}

void EnvComp::SetSky(std::string shader, std::string texPath) {
	RenderMesh* mesh = MeshGenerator::CreateSphere(800, 100, 100);
	mSkyItem = new RenderItem();
	mSkyItem->Material = MaterialManager::GetMaterialInstance(shader);
	mSkyItem->Material->SetCullMode(RS_CULL_FRONT);
	mSkyItem->Mesh = mesh;
	mSkyItem->SetMeshIndexGroup(0);

	Image* img = Image::Load(texPath);
	RHITexture* tex = GDevice->CreateTexture2D(img->GetFormat(), img->GetWidth(), img->GetHeight(), 1, img->GetData());
	mSkyItem->Material->SetParameter("tSkyTexture", tex);
	mSkyItem->WorldMatrix = math::Matrix4F::Identity;
}


void EnvComp::CollectRender(SceneCollection* collection) {
	// just get forward
	math::Matrix4F r = math::Matrix4F::Identity;
	r = r * math::MatrixRotationY(mSunRotater.y) * math::MatrixRotationX(mSunRotater.x) * math::MatrixRotationZ(mSunRotater.z);

	collection->ShaderParams[SP_SUN_COLOR] = math::Vector4F(mSunColor, 1.0f);
	collection->ShaderParams[SP_SUN_DIRECTION] = { r[2], 1.0f };
	collection->ShaderParams[SP_AMBIENT_COLOR] = math::Vector4F(mAmbientColor, 1.0f);

	if (mSkyItem) {
		collection->PushRenderItem(mSkyItem);
	}
}

}