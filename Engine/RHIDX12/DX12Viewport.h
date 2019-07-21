#pragma once

#include <RHI/RHIResource.h>

#include "DX12Header.h"
#include "DX12Texture.h"

namespace z {


static const uint32_t kBackBufferCount = 3;

using Microsoft::WRL::ComPtr;

class DX12Viewport : public RHIViewport {
public:
	DX12Viewport(uint32_t with, uint32_t height);

	void Init();
	void Resize(uint32_t width, uint32_t height);

private:
	uint32_t mWidth;
	uint32_t mHeight;

	RefCountPtr<IDXGISwapChain1> mSwapChain;
	std::array<RefCountPtr<DX12Texture2D>, kBackBufferCount> mBackBuffers;
};



}