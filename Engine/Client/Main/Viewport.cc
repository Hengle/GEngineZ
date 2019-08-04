#include "Viewport.h"
#include "Director.h"
#include "App.h"
#include <Core/CoreHeader.h>
#include <Client/Scene/Scene.h>
#include <Render/RenderScene.h>
#include <Render/Material.h>
#include <Render/RenderItem.h>

#include <RHI/RHIDevice.h>
#include <Util/Image/Image.h>
#include <Util/Mesh/ZMeshLoader.h>
#include <d2d1.h>
#include "GeometryGenerator.h"



namespace z {

struct VertexIn{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT3 tangent;
	DirectX::XMFLOAT2 uv;
};

struct PerObjectConstants {
	DirectX::XMFLOAT4X4 World;
};

struct PassConstants {
	DirectX::XMFLOAT4X4 ViewProj;
};


void Viewport::Resize(int width, int height) {
	mWidth = width;
	mHeight = height;

	mRHIViewport->Resize(width, height);
	ds = GDevice->CreateDepthStencil(width, height, PF_D24S8);
}

void Viewport::DrawTex() {
	float vertexs[] = {
		-1.0f, -1.0f, 0.1f, 0.0f, 1.0f,
		-1.0f, +1.0f, 0.1f, 0.0f, 0.0f,
		+1.0f, +1.0f, 0.1f, 1.0f, 0.0f,
		+1.0f, -1.0f, 0.1f, 1.0f, 1.0f,
	};

	uint16_t indices[] = {
		0,  1,  2,
		0,  2,  3,
	};


	RHIVertexBuffer *vb = GDevice->CreateVertexBuffer(4, 20, vertexs);
	RHIIndexBuffer* ib = GDevice->CreateIndexBuffer(6, 2, indices);


	MaterialInstance* mi = MaterialManager::GetMaterialInstance("render_tex");
	mi->SetParameter("gDiffuseMap", ds);

	
	mRHIViewport->SetRenderRect(ScreenRenderRect{ 0, 0, 200, 150 });

	GDevice->SetOutputs({ mRHIViewport->GetBackBuffer() }, nullptr);
	GDevice->SetVertexBuffer(vb);
	GDevice->SetIndexBuffer(ib);
	mi->DrawIndexed();


}

Viewport::Viewport(uint32_t width, uint32_t height) :
	mWidth(width), 
	mHeight(height) {
	MaterialManager::LoadShaders(GApp->GetRootPath() / "Content"/ "Engine" / "Shader" / "hlsl");
	
	mRenderScene = new RenderScene();

	mRHIViewport = GDevice->CreateViewport(width, height, PF_R8G8B8A8);

	mesh = ZMeshLoader::Load(GApp->GetRootPath() / "Content" / "Test" / "Mesh" / "nanosuit.zmesh");
	for (int i = 0; i < mesh->GetSubMeshNum(); i++) {
		RenderItem *item = new RenderItem();

		item->material = MaterialManager::GetMaterialInstance("test");
		item->mesh = mesh->GetSubMeshInstance(i, item->material->GetMaterial()->GetFVFs(), 2);
		items.push_back(item);
	}
	

	
	// texture
	Image* image = Image::Load(GApp->GetRootPath() / "Content/Test/WoodCrate01.tga");
	RHITextureDesc desc;
	desc.sizeX = image->GetWidth();
	desc.sizeY = image->GetHeight();
	desc.sizeZ = 0;
	desc.numMips = 1;
	desc.format = image->GetFormat();
	tex = GDevice->CreateTexture(desc, image->GetData());

	// depth stencil
	ds = GDevice->CreateDepthStencil(width, height, PF_D24S8);
	rt = GDevice->CreateRenderTarget(width, height, PF_R8G8B8A8);
	rt->Clear(RHIClearValue(0.2f, 0.3f, 0.3f, 1.0f));

}

Viewport::~Viewport() {
	SAFE_DELETE(mRenderScene);
 }

void Viewport::Tick() {
	// reset render scene
	mRenderScene->Begin();
	
	// init render scene, get view

	// culling

	// collect here

}

void Viewport::Render() {
	// render

	mRenderScene->Render();

	mRHIViewport->BeginDraw(RHIClearValue(0.3, 0.2, 0.5, 1.0));

	DirectX::XMVECTOR pos = DirectX::XMVectorSet(36.f, 16.f, 10.f, 1.0f);
	DirectX::XMVECTOR target = DirectX::XMVectorZero();
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	DirectX::XMMATRIX view_ = DirectX::XMMatrixLookAtLH(pos, target, up);

	float aspect = static_cast<float>(mWidth) / mHeight;
	DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(0.25f * DirectX::XM_PI, aspect, 1.0f, 1000.0f);

	PassConstants passConstans;
	XMStoreFloat4x4(&passConstans.ViewProj, DirectX::XMMatrixTranspose(view_ * proj));

	
	// world matrix for each object
	PerObjectConstants objConstants;
	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
	XMStoreFloat4x4(&objConstants.World, DirectX::XMMatrixTranspose(world));
	

	ds->Clear(RHIClearValue(1.0, 0));
	GDevice->SetOutputs({ mRHIViewport->GetBackBuffer() }, ds);

	for (auto item : items) {
		item->mesh->Use();
		item->material->SetParameter("gViewProj", (const float*)& passConstans.ViewProj, 16);
		item->material->SetParameter("gWorld", (const float*)& objConstants.World, 16);
		item->material->DrawIndexed();
	}
	//DrawTex();

	mRHIViewport->EndDraw();
}

}