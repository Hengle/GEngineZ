#include "EnvComp.h"
#include <Render/RenderItem.h>
#include <RHI/RHIResource.h>
#include <Render/RenderScene.h>
#include <Util/Mesh/MeshGenerator.h>
#include <Util/Image/Image.h>
#include <RHI/RHIDevice.h>

namespace z {

EnvComp::EnvComp() {

}

void EnvComp::SetSky(std::string shader, std::string texPath) {
	RenderMesh* mesh = MeshGenerator::CreateSphere(800, 100, 100);
	mSkyItem = new RenderItem();
	mSkyItem->Material = MaterialManager::GetMaterialInstance(shader);
	mSkyItem->Material->SetCullMode(RS_CULL_FRONT);
	mSkyItem->Mesh = mesh;
	mSkyItem->SetMeshIndexGroup(0);

	Image* img = Image::Load(texPath);
	RHITextureDesc desc;
	desc.sizeX = img->GetWidth();
	desc.sizeY = img->GetHeight();
	desc.sizeZ = 1;
	desc.flags = 0;
	desc.format = img->GetFormat();
	desc.dimension = TEX_DIMENSION_2D;
	desc.numMips = 1;
	RHITexture* tex = GDevice->CreateTexture(desc, img->GetData());
	mSkyItem->Material->SetParameter("tSkyTexture", tex);
	mSkyItem->WorldMatrix = math::Matrix4F::Identity;
}


void EnvComp::CollectRenderItems(RenderScene* scn) {
	if (mSkyItem) {
		scn->RenderItems.push_back(mSkyItem);
	}
}

}