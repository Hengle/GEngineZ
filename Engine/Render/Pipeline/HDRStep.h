#pragma once
#include <Core/CoreHeader.h>
#include <Render/Material.h>
#include "BaseScreenStep.h"

#include <Util/Image/Image.h>

namespace z {

class HDRStep : public BaseScreenStep {
	void Init() override {
		// create material
		mMaterial = MaterialManager::GetMaterialInstance("ToneMapping");

	}
	
	void Resize(uint32_t width, uint32_t height) override {
	}

	RenderTarget* Render(Renderer* r, RenderTarget* src, RenderTarget* dst) override {
		RenderStageScope stageScope("HDR");

		mMaterial->SetParameter("tBaseMap", src->GetRHIRenderTarget());

		dst->Clear({ 0.f, 0.f, 0.f, 1.f });
		DrawScreen(dst, mMaterial);

		return dst;
	}

private:
	RefCountPtr<MaterialInstance> mMaterial;
};


}