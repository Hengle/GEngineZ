#include "Viewport.h"
#include <Core/CoreHeader.h>
#include <Client/Scene/Scene.h>
#include <Render/RenderScene.h>

#include <RHIDX12/DX12Viewport.h>
#include <RHIDX12/DX12Shader.h>
#include <RHIDX12/DX12Buffer.h>

namespace z {

Viewport::Viewport(uint32_t width, uint32_t height) {
	mRenderScene = new RenderScene();

	view = new DX12Viewport(width, height);
	view->Init();

	// compile shader
	std::string s = FileReader("E:/Code/GameZ/Engine/Shader/test.hlsl").ReadAll();
	RefCountPtr<DX12Shader> vs = DX12Shader::FromCompile(s.c_str(), s.length(), SHADER_TYPE_VERTEX);
	RefCountPtr<DX12Shader> ps = DX12Shader::FromCompile(s.c_str(), s.length(), SHADER_TYPE_PIXEL);

	// vertex layout
	RefCountPtr<DX12VertexLayout> vertexLayout = new DX12VertexLayout();
	vertexLayout->PushLayout("POSITION", 0, VERTEX_FORMAT_R32G32B32_FLOAT, VERTEX_LAYOUT_PER_VERTEX);
	vertexLayout->PushLayout("COLOR", 0, VERTEX_FORMAT_R32G32B32A32_FLOAT, VERTEX_LAYOUT_PER_VERTEX);

	// uniform layout
	RefCountPtr<DX12UniformLayout> uniformLayout = new DX12UniformLayout();
	uniformLayout->PushLayout("globalcb", 0, UNIFORM_LAYOUT_CONSTANT_BUFFER);
	uniformLayout->PushLayout("privatecb", 0, UNIFORM_LAYOUT_CONSTANT_BUFFER);
	uniformLayout->GetRootSignature();

	struct CommonCBuffer { int x; };
	struct PrivateCBuffer { int y; };
	DX12ConstantBuffer *cbCommon = new DX12ConstantBuffer(sizeof(CommonCBuffer));
	DX12ConstantBuffer *cbPrivate= new DX12ConstantBuffer(sizeof(PrivateCBuffer));
	
	// ConstantBuffer1, ConstantBuffer2
	// U

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

	view->BeginDraw();
	view->EndDraw();
}

}