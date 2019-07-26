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

	DX12RenderTargetView* GetRenderTargetView(int index) {
		return mRenderTargets[index].GetRef();
	}

	void SetShaderResourceView(DX12ShaderResourceView* view) {
		mShaderResourceView = view;
	}



	RefCountPtr<DX12ShaderResourceView> mShaderResourceView;

private:

	std::vector<RefCountPtr<DX12RenderTargetView>> mRenderTargets;
};


class DX12Texture2D : public RHITexture2D, public DX12TextureBase {
public:
	DX12Texture2D(RHITextureDesc const& desc) : 
		mDesc(desc),
		RHITexture2D(),
		DX12TextureBase() {

	}

	const RHITextureDesc& GetDesc() {
		return mDesc;
	}

private:
	RHITextureDesc mDesc;

};





}
