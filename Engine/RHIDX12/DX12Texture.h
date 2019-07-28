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
	DX12TextureBase(const D3D12_RESOURCE_DESC&);

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

	void SetShaderResourceView(DX12ShaderResourceView* view) {
		mSRView = view;
	}

	DX12ShaderResourceView* GetShaderResourceView() {
		return mSRView.GetRef();
	}

	virtual void Clear(const D3D12_CLEAR_VALUE& value) = 0;

protected:
	uint32_t mWidth;
	uint32_t mHeight;
	uint32_t mDepth;
	uint32_t mNumMips;
	//uint32_t numSamples;
	DXGI_FORMAT mFormat;
	uint32_t mFlags;

	RefCountPtr<DX12ShaderResourceView> mSRView;
};


class DX12Texture2D : public DX12TextureBase, public RHITexture2D {
public:
	DX12Texture2D(const D3D12_RESOURCE_DESC& desc, const uint8_t* data);


	void Clear(const D3D12_CLEAR_VALUE& value) {};

protected:
	DX12ResourceOwner mUploader;
};


class DX12DepthStencil : public DX12TextureBase, public RHIDepthStencil {
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


class DX12RenderTarget :public DX12TextureBase, public RHIRenderTarget {
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
