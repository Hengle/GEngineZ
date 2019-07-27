#include "Viewport.h"
#include <Core/CoreHeader.h>
#include <Client/Scene/Scene.h>
#include <Render/RenderScene.h>

#include <RHI/RHIDevice.h>

#include <RHIDX12/DX12Viewport.h>
#include <RHIDX12/DX12Shader.h>
#include <RHIDX12/DX12Buffer.h>
#include <RHIDX12/DX12PipelineState.h>
#include <RHIDX12/DX12Device.h>
#include <RHIDX12/DX12Executor.h>

namespace z {

struct VertexIn{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT4 color;
};

struct PerObjectConstants {
	DirectX::XMFLOAT4X4 World;
};

struct PassConstants {
	DirectX::XMFLOAT4X4 ViewProj;
};

Viewport::Viewport(uint32_t width, uint32_t height) {
	mRenderScene = new RenderScene();

	viewport = GDevice->CreateViewport(width, height, PIXEL_FORMAT_R8G8B8A8_UNORM);

	// compile shader
	std::string s = FileReader("E:/Code/GameZ/Engine/Shader/test.hlsl").ReadAll();
	RHIShader *vs = GDevice->CreateShader(s.c_str(), s.length(), SHADER_TYPE_VERTEX);
	RHIShader *ps = GDevice->CreateShader(s.c_str(), s.length(), SHADER_TYPE_PIXEL);

	// vertex layout
	RHIVertexLayout *vl = new DX12VertexLayout();
	vl->PushLayout("POSITION", 0, PIXEL_FORMAT_R32G32B32_FLOAT, VERTEX_LAYOUT_PER_VERTEX);
	vl->PushLayout("COLOR", 0, PIXEL_FORMAT_R32G32B32A32_FLOAT, VERTEX_LAYOUT_PER_VERTEX);

	// uniform layout
	RHIUniformLayout *ul = new DX12UniformLayout();
	ul->PushLayout("globalcb", 0, UNIFORM_LAYOUT_CONSTANT_BUFFER);
	ul->PushLayout("privatecb", 1, UNIFORM_LAYOUT_CONSTANT_BUFFER);

	cb1 = new DX12ConstantBuffer(sizeof(PassConstants));
	cb0 = new DX12ConstantBuffer(sizeof(PerObjectConstants));
	cb2 = new DX12ConstantBuffer(sizeof(PerObjectConstants));

	RHIPipelineStateDesc psoDesc;
	psoDesc.vs = vs;
	psoDesc.ps = ps;
	psoDesc.ulayout = ul;
	psoDesc.vlayout = vl;
	psoDesc.rtsFormat = std::vector<ERHIPixelFormat>{ PIXEL_FORMAT_R8G8B8A8_UNORM };
	psoDesc.dsFormat = PIXEL_FORMAT_D24_UNORM_S8_UINT;
	state = GDevice->CreatePipelineState(psoDesc);
	
	const std::array<VertexIn, 8> kVertices{
			VertexIn({ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(DirectX::Colors::White) }),
			VertexIn({ DirectX::XMFLOAT3(-1.0f, +1.0f, -1.0f), DirectX::XMFLOAT4(DirectX::Colors::Black) }),
			VertexIn({ DirectX::XMFLOAT3(+1.0f, +1.0f, -1.0f), DirectX::XMFLOAT4(DirectX::Colors::Red) }),
			VertexIn({ DirectX::XMFLOAT3(+1.0f, -1.0f, -1.0f), DirectX::XMFLOAT4(DirectX::Colors::Green) }),
			VertexIn({ DirectX::XMFLOAT3(-1.0f, -1.0f, +1.0f), DirectX::XMFLOAT4(DirectX::Colors::Blue) }),
			VertexIn({ DirectX::XMFLOAT3(-1.0f, +1.0f, +1.0f), DirectX::XMFLOAT4(DirectX::Colors::Yellow) }),
			VertexIn({ DirectX::XMFLOAT3(+1.0f, +1.0f, +1.0f), DirectX::XMFLOAT4(DirectX::Colors::Cyan) }),
			VertexIn({ DirectX::XMFLOAT3(+1.0f, -1.0f, +1.0f), DirectX::XMFLOAT4(DirectX::Colors::Magenta) })
	};

	const std::array<std::uint16_t, 36> kIndices = {
		0, 1, 2, 0, 2, 3, // front face
		4, 6, 5, 4, 7, 6, // back face
		4, 5, 1, 4, 1, 0, // left face
		3, 2, 6, 3, 6, 7, // right face
		1, 5, 6, 1, 6, 2, // top face
		4, 0, 3, 4, 3, 7  // bottom face
	};


	vb = GDevice->CreateVertexBuffer(kVertices.size(), sizeof(VertexIn), kVertices.data());
	ib = GDevice->CreateIndexBuffer(kIndices.size(), 2, kIndices.data());


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

	ds->Clear(RHIClearValue(1.0, 0));
	GDevice->SetDepthStencil(ds);
	GDevice->SetPipelineState(state);
	GDevice->SetVertexBuffer(vb);
	GDevice->SetIndexBuffer(ib);
	GDevice->SetIndexBuffer(ib);
	GDevice->SetConstantBuffer(0, cb0);
	GDevice->SetConstantBuffer(1, cb1);

	GDevice->DrawIndexed();


	DirectX::XMMATRIX world2 = DirectX::XMMatrixTranslation(-3.0f, -1.5f, 0.0f);
	XMStoreFloat4x4(&objConstants.World, DirectX::XMMatrixTranspose(world2));
	cb2->CopyData(&objConstants, sizeof(PerObjectConstants));

	GDevice->SetConstantBuffer(0, cb2);

	GDevice->DrawIndexed();


	viewport->EndDraw();
}

}