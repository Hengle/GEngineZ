#pragma once
#include <Core/CoreHeader.h>
#include <RHI/RHIResource.h>
#include <RHI/RHIDevice.h>

#include "RenderTarget.h"
#include "Pipeline/RenderStep.h"

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
	
	//void RenderItems(ERenderSet set);



private:

	void CollectMaterialParametes(RenderItem*);

	RefCountPtr<RHIViewport> mRHIViewport;
	uint32_t mViewportWidth;
	uint32_t mViewportHeight;

	RefCountPtr<RenderTarget> mBackRT;
	RefCountPtr<DepthStencil> mBackDS;

	RefCountPtr<RenderScene> mRenderScene;

	std::unordered_map<ERenderStep, RefCountPtr<RenderStep>> mRenderSteps;



	

};

}