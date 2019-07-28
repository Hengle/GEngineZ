#pragma once
#include "DX12Header.h"
#include "DX12Resource.h"
#include "DX12Descriptor.h"
#include <RHI/RHIResource.h>
#include <Core/CoreHeader.h>

namespace z {

class DX12Texture : public DX12ShaderResource, public RHITexture {
public:
	DX12Texture();
	DX12Texture(const RHITextureDesc&);
	void InitWithResourceDesc(const D3D12_RESOURCE_DESC&);
	void InitWithRHITextureDesc(const RHITextureDesc&);

	void SetSRView(DX12ShaderResourceView* view) {
		mSRView = view;
	}

	DX12ShaderResourceView* GetSRView() {
		return mSRView;
	}

	void AttachResource(DX12Resource* resource);

	virtual void Clear(const D3D12_CLEAR_VALUE& value) {}

	virtual void SetWritable() {}
	virtual void SetReadable() {}

	// RHI begin
	virtual void Clear(const RHIClearValue& v) override {
		Clear(FromRHIClearVale(v));
	}
	// RHI end

public:
	uint32_t width;
	uint32_t height;
	uint32_t depth;
	uint32_t numMips;
	DXGI_FORMAT format;
	// dimension
	// sampler

protected:

	RefCountPtr<DX12ShaderResourceView> mSRView;
};


class DX12Texture2D : public DX12Texture {
public:
	DX12Texture2D(const RHITextureDesc& desc, const uint8_t* data);



protected:
	DX12ResourceOwner mUploader;
};


// Depth Stencil 
class DX12DepthStencil : public DX12Texture {
public:
	DX12DepthStencil(uint32_t width, uint32_t height, DXGI_FORMAT format);
	
	DX12DepthStencilView* GetDSView() {
		return mDSView;
	}


	void SetWritable() override;
	void SetReadable() override;
	void Clear(const D3D12_CLEAR_VALUE& value) override;


private:
	RefCountPtr<DX12DepthStencilView> mDSView;
};


// Render Target
class DX12RenderTarget :public DX12Texture {
public:
	DX12RenderTarget(DX12Resource* resource);

	DX12RenderTargetView* GetRTView() {
		return mRTView;
	}


	void SetPresent();
	void SetWritable() override;
	void SetReadable() override;
	void Clear(const D3D12_CLEAR_VALUE &value) override;

private:
	RefCountPtr<DX12RenderTargetView> mRTView;
};





}
