#pragma once
#include <Core/CoreHeader.h>
#include <RHI/RHIResource.h>
#include <RHI/RHIDevice.h>

namespace z {
class RenderScene;
class RenderItem;

class Renderer : public RefCounter {
public:
	Renderer();
	virtual ~Renderer();

	void Resize(uint32_t width, uint32_t height);
	void Tick();
	void Render();


private:
	void CollectMaterialParametes(RenderItem*);

	RefCountPtr<RHITexture> mDepthStencil;
	RefCountPtr<RHIViewport> mRHIViewport;
	uint32_t mViewportWidth;
	uint32_t mViewportHeight;

	RefCountPtr<RenderScene> mRenderScene;

};

}