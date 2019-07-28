#include "Viewport.h"
#include <Core/CoreHeader.h>
#include <Client/Scene/Scene.h>
#include <Render/RenderScene.h>

#include <RHI/RHIDevice.h>
#include <Util/Image/Image.h>

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

	std::string s = FileReader("E:/Code/GameZ/Content/Engine/Shader/render_tex.hlsl").ReadAll();
	RHIShader* vs = GDevice->CreateShader(s.c_str(), s.length(), SHADER_TYPE_VERTEX);
	RHIShader* ps = GDevice->CreateShader(s.c_str(), s.length(), SHADER_TYPE_PIXEL);

	RHIVertexLayout* vl = GDevice->CreateVertexLayout();
	vl->PushLayout("POSITION", 0, PIXEL_FORMAT_R32G32B32_FLOAT, VERTEX_LAYOUT_PER_VERTEX);
	vl->PushLayout("TEXCOORD", 0, PIXEL_FORMAT_R32G32_FLOAT, VERTEX_LAYOUT_PER_VERTEX);
	
	RHIUniformLayout* ul = GDevice->CreateUniformLayout();
	ul->PushLayout("texture0", 0, UNIFORM_LAYOUT_TEXTURE);


	RHIPipelineStateDesc psoDesc;
	psoDesc.vs = vs;
	psoDesc.ps = ps;
	psoDesc.ulayout = ul;
	psoDesc.vlayout = vl;
	psoDesc.rtsFormat = std::vector<ERHIPixelFormat>{ PIXEL_FORMAT_R8G8B8A8_UNORM };
	psoDesc.dsFormat = PIXEL_FORMAT_INVALID;
	RHIPipelineState *state = GDevice->CreatePipelineState(psoDesc);

	GDevice->SetPipelineState(state);
	GDevice->SetDepthStencil(nullptr);

	GDevice->SetVertexBuffer(vb);
	GDevice->SetIndexBuffer(ib);
	GDevice->SetTexture(0, ds);

	viewport->SetRenderRect(ScreenRenderRect{ 0, 0, 200, 150 });
	GDevice->DrawIndexed();



}

Viewport::Viewport(uint32_t width, uint32_t height) {
	mRenderScene = new RenderScene();

	viewport = GDevice->CreateViewport(width, height, PIXEL_FORMAT_R8G8B8A8_UNORM);

	// vertex and indices
	GeometryGenerator geoGen;
	GeometryGenerator::MeshData box = geoGen.CreateBox(2.0f, 2.0f, 2.0f, 3);
	vb = GDevice->CreateVertexBuffer(box.Vertices.size(), sizeof(VertexIn), box.Vertices.data());
	ib = GDevice->CreateIndexBuffer(box.GetIndices16().size(), 2, box.GetIndices16().data());

	// pipeline
	std::string s = FileReader("E:/Code/GameZ/Content/Engine/Shader/test.hlsl").ReadAll();
	RHIShader *vs = GDevice->CreateShader(s.c_str(), s.length(), SHADER_TYPE_VERTEX);
	RHIShader *ps = GDevice->CreateShader(s.c_str(), s.length(), SHADER_TYPE_PIXEL);

	RHIVertexLayout* vl = GDevice->CreateVertexLayout();
	vl->PushLayout("POSITION", 0, PIXEL_FORMAT_R32G32B32_FLOAT, VERTEX_LAYOUT_PER_VERTEX);
	vl->PushLayout("NORMAL", 0, PIXEL_FORMAT_R32G32B32_FLOAT, VERTEX_LAYOUT_PER_VERTEX);
	vl->PushLayout("TANGENT", 0, PIXEL_FORMAT_R32G32B32_FLOAT, VERTEX_LAYOUT_PER_VERTEX);
	vl->PushLayout("TEXCOORD", 0, PIXEL_FORMAT_R32G32_FLOAT, VERTEX_LAYOUT_PER_VERTEX);

	RHIUniformLayout* ul = GDevice->CreateUniformLayout();
	ul->PushLayout("globalcb", 0, UNIFORM_LAYOUT_CONSTANT_BUFFER);
	ul->PushLayout("privatecb", 1, UNIFORM_LAYOUT_CONSTANT_BUFFER);
	ul->PushLayout("texture0", 0, UNIFORM_LAYOUT_TEXTURE);


	RHIPipelineStateDesc psoDesc;
	psoDesc.vs = vs;
	psoDesc.ps = ps;
	psoDesc.ulayout = ul;
	psoDesc.vlayout = vl;
	psoDesc.rtsFormat = std::vector<ERHIPixelFormat>{ PIXEL_FORMAT_R8G8B8A8_UNORM };
	psoDesc.dsFormat = PIXEL_FORMAT_D24_UNORM_S8_UINT;
	state = GDevice->CreatePipelineState(psoDesc);

	// constant buffer
	cb1 = GDevice->CreateConstantBuffer(sizeof(PassConstants));
	cb0 = GDevice->CreateConstantBuffer(sizeof(PerObjectConstants));
	cb2 = GDevice->CreateConstantBuffer(sizeof(PerObjectConstants));

	// texture
	Image* image = Image::Load("E:/Code/GameZ/Content/Test/WoodCrate01.tga");
	RHITextureDesc desc;
	desc.sizeX = image->GetWidth();
	desc.sizeY = image->GetHeight();
	desc.sizeZ = 0;
	desc.numMips = 1;
	desc.format = image->GetFormat();
	tex = GDevice->CreateTexture(desc, image->GetData());

	// depth stencil
	ds = GDevice->CreateDepthStencil(width, height, PIXEL_FORMAT_D24_UNORM_S8_UINT);
}

Viewport::~Viewport() {
	SAFE_DELETE(mRenderScene);
 }

void Viewport::Tick() {
	// reset render scene
	mRenderScene->Begin();
	
	// init render scene, get view

	// culling

}

void Viewport::PostTick() {
	// collect here
	mScene->ColloectEnv(mRenderScene);
}

void Viewport::Render() {
	// render

	mRenderScene->Render();

	viewport->BeginDraw(RHIClearValue(0.3, 0.2, 0.5, 1.0));

	DirectX::XMVECTOR pos = DirectX::XMVectorSet(6.f, 2.f, 10.f, 1.0f);
	DirectX::XMVECTOR target = DirectX::XMVectorZero();
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	DirectX::XMMATRIX view_ = DirectX::XMMatrixLookAtLH(pos, target, up);

	float aspect = static_cast<float>(800) / 600;
	DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(0.25f * DirectX::XM_PI, aspect, 1.0f, 1000.0f);

	PassConstants passConstans;
	XMStoreFloat4x4(&passConstans.ViewProj, DirectX::XMMatrixTranspose(view_ * proj));
	cb1->CopyData(&passConstans, sizeof(PassConstants));

	// world matrix for each object
	PerObjectConstants objConstants;
	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
	XMStoreFloat4x4(&objConstants.World, DirectX::XMMatrixTranspose(world));
	cb0->CopyData(&objConstants, sizeof(PerObjectConstants));

	DirectX::XMMATRIX world2 = DirectX::XMMatrixTranslation(-3.0f, -1.5f, 0.0f);
	XMStoreFloat4x4(&objConstants.World, DirectX::XMMatrixTranspose(world2));
	cb2->CopyData(&objConstants, sizeof(PerObjectConstants));

	ds->Clear(RHIClearValue(1.0, 0));
	
	GDevice->SetPipelineState(state);
	GDevice->SetDepthStencil(ds);
	GDevice->SetVertexBuffer(vb);
	GDevice->SetIndexBuffer(ib);
	GDevice->SetIndexBuffer(ib);
	GDevice->SetConstantBuffer(0, cb0);
	GDevice->SetConstantBuffer(1, cb1);
	GDevice->SetTexture(0, tex);

	GDevice->DrawIndexed();

	GDevice->SetConstantBuffer(0, cb2);
	GDevice->DrawIndexed();

	DrawTex();


	viewport->EndDraw();
}

}