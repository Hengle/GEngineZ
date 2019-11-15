#pragma once

#include <RHI/RHIResource.h>

#include "DX12Header.h"
#include "DX12Texture.h"

namespace z {




class DX12Viewport : public RHIViewport {
public:
	DX12Viewport(uint32_t with, uint32_t height, DXGI_FORMAT format, void *window);
	
	// begin RHI Viewport
	void Resize(uint32_t width, uint32_t height) override;
	void BeginDraw() override;
	void EndDraw() override;
	RHITexture* GetBackBuffer() override {
		return GetCurBackBuffer();
	}
	

	// End RHI Viewport

	DX12RenderTarget* GetCurBackBuffer() {
		return mBackBuffers[mCurBackBufferIndex];
	}

private:
	void Present();

	uint32_t mWidth;
	uint32_t mHeight;
	DXGI_FORMAT mFormat;

	// swapchain
	RefCountPtr<IDXGISwapChain1> mSwapChain;
	std::array<RefCountPtr<DX12RenderTarget>, BACK_BUFFER_COUNT> mBackBuffers;
	int32_t mCurBackBufferIndex;
};



}