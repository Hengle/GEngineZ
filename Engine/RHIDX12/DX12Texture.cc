#include "DX12Texture.h"
#include "DX12Device.h"
#include "DX12Executor.h"

namespace z {

// DX12Texture
DX12Texture::DX12Texture() :
	width(0), 
	height(0), 
	depth(0), 
	numMips(0), 
	format(DXGI_FORMAT_UNKNOWN) {

}

DX12Texture::DX12Texture(const RHITextureDesc& desc) {
	InitWithRHITextureDesc(desc);
}

void DX12Texture::InitWithResourceDesc(const D3D12_RESOURCE_DESC& desc) {
	width = desc.Width;
	height = desc.Height;
	depth = desc.DepthOrArraySize;
	numMips = desc.MipLevels;
	format = desc.Format;
}


void DX12Texture::InitWithRHITextureDesc(const RHITextureDesc& desc) {
	width = desc.sizeX;
	height = desc.sizeY;
	depth = desc.sizeZ;
	numMips = desc.numMips;
	format = FromRHIFormat(desc.format);
}


void DX12Texture::AttachResource(DX12Resource* resource) {
	GetResourceOwner()->OwnResource(EResourceOwn_Exclusive, resource);
}

// DX12Texture2D
DX12Texture2D::DX12Texture2D(const RHITextureDesc& desc, const uint8_t* data) :
	DX12Texture(desc) {
	auto exec = GDX12Device->GetExecutor();

	// create resource
	D3D12_RESOURCE_DESC texDesc;
	texDesc.Dimension          = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	texDesc.Alignment          = 0;
	texDesc.Width              = width;
	texDesc.Height             = height;
	texDesc.DepthOrArraySize   = 1;
	texDesc.MipLevels          = numMips;
	texDesc.Format             = format;
	texDesc.SampleDesc.Count   = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Layout             = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	texDesc.Flags              = D3D12_RESOURCE_FLAG_NONE;
	RefCountPtr<DX12Resource> uploader, dest;
	dest = new DX12Resource(D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_COMMON, texDesc);
	AttachResource(dest);

	D3D12_SUBRESOURCE_DATA subResourceData = {};
	subResourceData.pData = data;
	subResourceData.RowPitch = (uint64_t)width * GetPixelSize(format);
	subResourceData.SlicePitch = (uint64_t)height * width * GetPixelSize(format);

	// creat uploader resource
	const uint64_t size = GetRequiredIntermediateSize(dest->GetIResource(), 0, 1);
	D3D12_RESOURCE_DESC uploaderDesc = CD3DX12_RESOURCE_DESC::Buffer(size);
	uploader = new DX12Resource(D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_GENERIC_READ, uploaderDesc);

	// copy
	dest->Transition(D3D12_RESOURCE_STATE_COPY_DEST);
	UpdateSubresources<1>(exec->GetCommandList(), dest->GetIResource(), uploader->GetIResource(), 0, 0, 1, &subResourceData);
	dest->Transition(D3D12_RESOURCE_STATE_GENERIC_READ);
	
	GetResourceOwner()->OwnResource(EResourceOwn_Exclusive, dest);
	mUploader.OwnResource(EResourceOwn_Exclusive, uploader);

	// create shader resource view
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format                        = format;
	srvDesc.ViewDimension                 = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Shader4ComponentMapping       = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Texture2D.MostDetailedMip     = 0;
	srvDesc.Texture2D.MipLevels           = numMips;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	srvDesc.Texture2D.PlaneSlice          = 0;
	SetSRView(new DX12ShaderResourceView(srvDesc, dest));
}


// DX12DepthStencil
DX12DepthStencil::DX12DepthStencil(uint32_t width, uint32_t height, DXGI_FORMAT format) :
	DX12Texture()  {
	D3D12_RESOURCE_DESC dsDesc;
	dsDesc.Dimension          = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	dsDesc.Alignment          = 0;
	dsDesc.Width              = width;
	dsDesc.Height             = height;
	dsDesc.DepthOrArraySize   = 1;
	dsDesc.MipLevels          = 1;
	dsDesc.Format             = format;
	dsDesc.SampleDesc.Count   = 1;
	dsDesc.SampleDesc.Quality = 0;
	dsDesc.Layout             = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	dsDesc.Flags              = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	// int texture info
	InitWithResourceDesc(dsDesc);
	// TODO, init sampler

	// create resource
	DX12Resource* resource = new DX12Resource(D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_COMMON, dsDesc);
	AttachResource(resource);

	// create depth stencil view
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format             = format;
	dsvDesc.ViewDimension      = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Flags              = D3D12_DSV_FLAG_NONE;
	dsvDesc.Texture2D.MipSlice = 0;
	mDSView = new DX12DepthStencilView(dsvDesc, resource);

	// create shader view
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format                        = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDesc.ViewDimension                 = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Shader4ComponentMapping       = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Texture2D.MostDetailedMip     = 0;
	srvDesc.Texture2D.MipLevels           = 1;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	srvDesc.Texture2D.PlaneSlice          = 0;
	SetSRView(new DX12ShaderResourceView(srvDesc, resource));
}

void DX12DepthStencil::SetWritable() {
	DX12Resource* resource = GetResourceOwner()->GetResource();
	if (resource->GetState() != D3D12_RESOURCE_STATE_DEPTH_WRITE) {
		resource->Transition(D3D12_RESOURCE_STATE_DEPTH_WRITE);
	}
}

void DX12DepthStencil::SetReadable() {
	DX12Resource* resource = GetResourceOwner()->GetResource();
	if (resource->GetState() != D3D12_RESOURCE_STATE_GENERIC_READ) {
		resource->Transition(D3D12_RESOURCE_STATE_GENERIC_READ);
	}
}

void DX12DepthStencil::Clear(const D3D12_CLEAR_VALUE& value) {
	SetWritable();
	GDX12Device->GetExecutor()->GetCommandList()->ClearDepthStencilView(mDSView->GetCPUHandle(), 
		D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, value.DepthStencil.Depth, value.DepthStencil.Stencil, 0, nullptr);
}


// DX12RenderTarget
DX12RenderTarget::DX12RenderTarget(DX12Resource* resource) {
	// int texture info
	InitWithResourceDesc(resource->GetDesc());
	// TODO, init sampler

	// attach
	AttachResource(resource);

	// create render target view
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format               = format;
	rtvDesc.ViewDimension        = D3D12_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice   = 0;
	rtvDesc.Texture2D.PlaneSlice = 0;
	mRTView = new DX12RenderTargetView(rtvDesc, resource);

	// create shader resource view
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format                        = format;
	srvDesc.ViewDimension                 = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Shader4ComponentMapping       = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Texture2D.MostDetailedMip     = 0;
	srvDesc.Texture2D.MipLevels           = 1;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	srvDesc.Texture2D.PlaneSlice          = 0;
	SetSRView(new DX12ShaderResourceView(srvDesc, resource));
}


void DX12RenderTarget::SetReadable() {
	DX12Resource* resource = GetResourceOwner()->GetResource();
	if (resource->GetState() != D3D12_RESOURCE_STATE_GENERIC_READ) {
		resource->Transition(D3D12_RESOURCE_STATE_GENERIC_READ);
	}
}

void DX12RenderTarget::SetWritable() {
	DX12Resource* resource = GetResourceOwner()->GetResource();
	if (resource->GetState() != D3D12_RESOURCE_STATE_RENDER_TARGET) {
		resource->Transition(D3D12_RESOURCE_STATE_RENDER_TARGET);
	}
}

void DX12RenderTarget::SetPresent() {
	DX12Resource* resource = GetResourceOwner()->GetResource();
	if (resource->GetState() != D3D12_RESOURCE_STATE_PRESENT) {
		resource->Transition(D3D12_RESOURCE_STATE_PRESENT);
	}
}

void DX12RenderTarget::Clear(const D3D12_CLEAR_VALUE& value) {
	SetWritable();
	GDX12Device->GetExecutor()->GetCommandList()->ClearRenderTargetView(mRTView->GetCPUHandle(), value.Color, 0, nullptr);
}



}\