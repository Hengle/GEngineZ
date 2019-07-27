#pragma once
#include "DX12Header.h"
#include "DX12Resource.h"
#include "DX12Descriptor.h"
#include <RHI/RHIResource.h>
#include <Core/CoreHeader.h>

namespace z {

class DX12TextureBase : public DX12ShaderResource {
public:
	DX12TextureBase();
	DX12TextureBase(const RHITextureDesc&);
	DX12TextureBase(const D3D12_RESOURCE_DESC&);

	void InitWithRHITexDesc(const RHITextureDesc&);
	void InitWithResouceDesc(const D3D12_RESOURCE_DESC&);
	
	DXGI_FORMAT GetFormat() {
		return mFormat;
	}

	uint32_t GetWidth() {
		return mWidth;
	}

	uint32_t GetHeight() {
		return mHeight;
	}

	uint32_t GetDepth() {
		return mDepth;
	}


	virtual void Clear(const D3D12_CLEAR_VALUE& value) = 0;


	/*	void SetShaderResourceView(DX12ShaderResourceView* view) {
		mShaderResourceView = view;
	}*/



protected:
	uint32_t mWidth;
	uint32_t mHeight;
	uint32_t mDepth;
	uint32_t mNumMips;
	//uint32_t numSamples;
	DXGI_FORMAT mFormat;
	uint32_t mFlags;
	D3D12_CLEAR_VALUE mClearValue;

	RefCountPtr<DX12ShaderResourceView> mShaderResourceView;
};


class DX12Texture2D : public RHITexture2D, public DX12TextureBase {
public:
	DX12Texture2D(RHITextureDesc const& desc) : 
		mDesc(desc),
		RHITexture2D(),
		DX12TextureBase(desc) {

	}

	const RHITextureDesc& GetDesc() {
		return mDesc;
	}

	void Clear(const D3D12_CLEAR_VALUE& value) {};
protected:
	RHITextureDesc mDesc;

};

class DX12DepthStencil : public RHIDepthStencil, public DX12TextureBase {
public:
	DX12DepthStencil(uint32_t width, uint32_t height, DXGI_FORMAT format);
	
	DX12DepthStencilView* GetView() {
		return mDSView;
	}

	void Clear(const D3D12_CLEAR_VALUE& value) override;

	// RHI begin
	virtual void Clear(const RHIClearValue& v) override {
		Clear(FromRHIClearVale(v));
	}

	// RHI end
private:
	RefCountPtr<DX12DepthStencilView> mDSView;
};

class DX12RenderTarget : public RHIRenderTarget, public DX12TextureBase {
public:
	// creat render target from exist resource
	DX12RenderTarget(DX12Resource* resource);

	// TODO
	// DX12RenderTarget(const RHITextureDesc& desc) {}

	DX12RenderTargetView* GetRTView() {
		return mRTView;
	}

	void Clear(const D3D12_CLEAR_VALUE &value) override;

private:
	RefCountPtr<DX12RenderTargetView> mRTView;
	RefCountPtr<DX12ShaderResourceView> mSRView;
};





}
