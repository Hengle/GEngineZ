#pragma once

#include <Core/CoreHeader.h>

#include <RHI/RHIResource.h>

namespace z {
class Scene;
class RenderScene;

class DX12Viewport;
class DX12PipelineState;
class DX12VertexBuffer;
class DX12IndexBuffer;
class DX12ConstantBuffer;
class DX12DepthStencil;

class Viewport {
public:
	Viewport(uint32_t width, uint32_t height);
	virtual ~Viewport();

	void Tick();
	void PostTick();
	void Render();


private:
	Scene* mScene;
	RenderScene* mRenderScene;

	RHIViewport* viewport;
	RefCountPtr<RHIPipelineState> state;
	RefCountPtr<RHIVertexBuffer> vb;
	RefCountPtr<RHIIndexBuffer> ib;
	RefCountPtr<RHIConstantBuffer> cb1, cb0, cb2;
	RefCountPtr<RHIDepthStencil> ds;
	

};


}