#pragma once
#include <Core/CoreHeader.h>
#include <RHI/RHIResource.h>
#include <RHI/RHIDevice.h>

#include "RenderTarget.h"
#include "SceneCollection.h"

#include "Pipeline/RenderStep.h"


namespace z {

class SceneCollection;
class RenderItem;

class Renderer : public RefCounter {
public:
	Renderer();
	virtual ~Renderer();

	void Resize(uint32_t width, uint32_t height);
	void Tick();
	void Render();
	
	SceneCollection* GetSceneCollection();

private:

	RefCountPtr<RHIViewport> mRHIViewport;
	uint32_t mViewportWidth;
	uint32_t mViewportHeight;

	RefCountPtr<RenderTarget> mBackRT;

	std::unordered_map<ERenderStep, RefCountPtr<RenderStep>> mRenderSteps;

	RefCountPtr<SceneCollection> mSceneCol;
	

};

}