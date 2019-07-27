#include "DX12Texture.h"
#include "DX12Device.h"
#include "DX12Executor.h"

namespace z {

// DX12TextureBase
DX12TextureBase::DX12TextureBase() {

}

DX12TextureBase::DX12TextureBase(const RHITextureDesc& desc) {
	InitWithRHITexDesc(desc);
}

DX12TextureBase::DX12TextureBase(const D3D12_RESOURCE_DESC& desc) {
	InitWithResouceDesc(desc);
}

void DX12TextureBase::InitWithRHITexDesc(const RHITextureDesc&) {

}

void DX12TextureBase::InitWithResouceDesc(const D3D12_RESOURCE_DESC& desc) {
	mWidth = desc.Width;
	mHeight = desc.Height;
	mFormat = desc.Format;
}


// DX12DepthStencil
DX12DepthStencil::DX12DepthStencil(uint32_t width, uint32_t height, DXGI_FORMAT format) :
	RHIDepthStencil(), DX12TextureBase() {
	D3D12_RESOURCE_DESC dsDesc;
	dsDesc.Dimension          = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	dsDesc.Alignment          = 0;
	dsDesc.Width              = width;
	dsDesc.Height             = width;
	dsDesc.DepthOrArraySize   = 1;
	dsDesc.MipLevels          = 1;
	dsDesc.Format             = format;
	dsDesc.SampleDesc.Count   = 1;
	dsDesc.SampleDesc.Quality = 0;
	dsDesc.Layout             = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	dsDesc.Flags              = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	// init texture base info
	InitWithResouceDesc(dsDesc);

	// create resource
	DX12Resource* resource = new DX12Resource(D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_COMMON, dsDesc);
	resource->Transition(D3D12_RESOURCE_STATE_DEPTH_WRITE);
	GetResourceOwner()->OwnResource(EResourceOwn_Exclusive, resource);

	// create view
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format             = mFormat;
	dsvDesc.ViewDimension      = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Flags              = D3D12_DSV_FLAG_NONE;
	dsvDesc.Texture2D.MipSlice = 0;
	mDSView = new DX12DepthStencilView(dsvDesc, resource);
}


void DX12DepthStencil::Clear(const D3D12_CLEAR_VALUE& value) {
	GDX12Device->GetExecutor()->GetCommandList()->ClearDepthStencilView(mDSView->GetCPUHandle(), 
		D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, value.DepthStencil.Depth, value.DepthStencil.Stencil, 0, nullptr);
}

// DX12RenderTarget
DX12RenderTarget::DX12RenderTarget(DX12Resource* resource) :
	DX12TextureBase(resource->GetDesc()) {
	GetResourceOwner()->OwnResource(EResourceOwn_Exclusive, resource);

	// create render target
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format             = mFormat;
	rtvDesc.ViewDimension      = D3D12_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;
	mRTView = new DX12RenderTargetView(rtvDesc, resource);

	// create shader resource
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format                    = mFormat;
	srvDesc.ViewDimension             = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Shader4ComponentMapping   = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels       = 1;
	mSRView = new DX12ShaderResourceView(srvDesc, resource);
}

void DX12RenderTarget::Clear(const D3D12_CLEAR_VALUE& value) {
	GDX12Device->GetExecutor()->GetCommandList()->ClearRenderTargetView(mRTView->GetCPUHandle(), value.Color, 0, nullptr);
}



}\