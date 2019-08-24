#pragma once
#include <Core/CoreHeader.h>
#include <RHI/RHIResource.h>
#include <RHI/RHIDevice.h>
#include <RHI/RHIConst.h>

namespace z {


class RenderTarget : public RefCounter {
public:
	RenderTarget(uint32_t width, uint32_t height, ERHIPixelFormat format) :
		mFormat(format) {
		mRT = GDevice->CreateRenderTarget(width, height, format);
	}

	RenderTarget(RHITexture* tex) {
		mRT = tex;
	}

	void Clear(const math::Vector4F &value) {
		mRT->Clear({ value[0], value[1], value[2], value[3] });
	}

	void SetSize(int width, int height) {

	}

	void Resize(int width, int height) {
		mRT = GDevice->CreateRenderTarget(width, height, mFormat);
	}

	RHITexture* GetRHIRenderTarget() {
		return mRT;
	}

private:
	RefCountPtr<RHITexture> mRT;
	ERHIPixelFormat mFormat;
};


class DepthStencil : public RefCounter {
public:
	DepthStencil(uint32_t width, uint32_t height, ERHIPixelFormat format = PF_D24S8) :
		mFormat(format) {
		mDS = GDevice->CreateDepthStencil(width, height, format);
	}

	void Clear(float depth, uint8_t stencil) {
		mDS->Clear({ depth, stencil });
	}

	void Resize(int width, int height) {
		mDS = GDevice->CreateDepthStencil(width, height, mFormat);
	}

	RHITexture* GetRHIDepthStencil() {
		return mDS;
	}

private:
	RefCountPtr<RHITexture> mDS;
	ERHIPixelFormat mFormat;
};


}