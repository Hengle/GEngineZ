#pragma once
#include "DX12Header.h"
#include "DX12Resource.h"
#include "DX12Descriptor.h"
#include <RHI/RHIResource.h>
#include <Core/CoreHeader.h>

namespace z {

class DX12TextureBase : public DX12ShaderResource {
public:
	DX12TextureBase() {}

	void SetRenderTargetView(DX12RenderTargetView* view) {
		mRenderTargets.emplace_back(view);
	}

	void SetShaderResourceView(DX12ShaderResourceView* view) {
		mShaderResourceView = view;
	}

	RefCountPtr<DX12ShaderResourceView> mShaderResourceView;
	std::vector<RefCountPtr<DX12RenderTargetView>> mRenderTargets;
};


class DX12Texture2D : public RHITexture2D, public DX12TextureBase {
public:
	DX12Texture2D(RHITextureDesc const& desc) : 
		RHITexture2D(),
		DX12TextureBase() {

	}
};





}
