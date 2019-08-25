#pragma once
#include <Core/CoreHeader.h>
#include <RHI/RHIDevice.h>
#include <RHI/RHIResource.h>
#include <Render/RenderItem.h>

#include "RenderStep.h"

namespace z {

class IMGuiStep : public RenderStep {
public:
	
	void Init() override;
	void Resize(uint32_t width, uint32_t height) override;

	RenderTarget* Render(Renderer* r, RenderTarget* src = nullptr, RenderTarget* dst=nullptr) override;

private:
	RefCountPtr<RenderItem> mItem;
	std::vector<RefCountPtr<RHITexture>> mTexs;

};

}