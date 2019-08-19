
#include "DX12Texture.h"
#include "DX12Device.h"
#include "DX12Executor.h"
#include "DX12Buffer.h"

namespace z {

// DX12Sampler
DX12Sampler::DX12Sampler(uint32_t samplerFlag) :
	mSamplerFlag (samplerFlag) {
	D3D12_FILTER filter_mode = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	D3D12_TEXTURE_ADDRESS_MODE address_mode = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	switch (samplerFlag & 0XFF) {
	case SAMPLER_FILTER_LINEAR:
		filter_mode = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		break;
	case SAMPLER_FILTER_POINT:
		filter_mode = D3D12_FILTER_MIN_MAG_MIP_POINT;
		break;
	case SAMPLER_FILTER_ANISOTROPIC:
		filter_mode = D3D12_FILTER_ANISOTROPIC;
		break;
	}

	switch (samplerFlag & 0xFF00) {
	case SAMPLER_ADDRESS_WRAP:
		address_mode = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		break;
	case SAMPLER_ADDRESS_MIRROR:
		address_mode = D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
		break;
	case SAMPLER_ADDRESS_CLAMP:
		address_mode = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		break;
	case SAMPLER_ADDRESS_BORDER:
		address_mode = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
		break;
	}

	D3D12_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = filter_mode;
	samplerDesc.AddressU = address_mode;
	samplerDesc.AddressV = address_mode;
	samplerDesc.AddressW = address_mode;
	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	samplerDesc.BorderColor[0] = 0.f;
	samplerDesc.BorderColor[1] = 0.f;
	samplerDesc.BorderColor[2] = 0.f;
	samplerDesc.BorderColor[3] = 0.f;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;

	mView = new DX12SamplerView(samplerDesc);
}


// DX12Texture
DX12Texture::DX12Texture() :
	width(0), 
	height(0), 
	depth(0), 
	numMips(0), 
	Format(DXGI_FORMAT_UNKNOWN) {
}

DX12Texture::DX12Texture(const RHITextureDesc& desc) {
	InitWithRHITextureDesc(desc);
}

void DX12Texture::InitWithResourceDesc(const D3D12_RESOURCE_DESC& desc) {
	width = (uint32_t)desc.Width;
	height = desc.Height;
	depth = desc.DepthOrArraySize;
	numMips = desc.MipLevels;
	Format = desc.Format;
}


void DX12Texture::InitWithRHITextureDesc(const RHITextureDesc& desc) {
	width = desc.SizeX;
	height = desc.SizeY;
	depth = desc.SizeZ;
	numMips = desc.NumMips;
	Format = FromRHIFormat(desc.Format);
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
	texDesc.Format             = Format;
	texDesc.SampleDesc.Count   = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Layout             = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	texDesc.Flags              = D3D12_RESOURCE_FLAG_NONE;
	mResource = new DX12Resource(D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_COMMON, texDesc);

	DX12BufferUploader::UploadTexture(mResource, data, width, height, GetPixelSize(Format));
	
	// create shader resource view
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format                        = Format;
	srvDesc.ViewDimension                 = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Shader4ComponentMapping       = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Texture2D.MostDetailedMip     = 0;
	srvDesc.Texture2D.MipLevels           = numMips;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	srvDesc.Texture2D.PlaneSlice          = 0;
	SetSRView(new DX12ShaderResourceView(srvDesc, mResource));
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
	
	// create resource
	mResource = new DX12Resource(D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_COMMON, dsDesc);
	
	// create depth stencil view
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format             = format;
	dsvDesc.ViewDimension      = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Flags              = D3D12_DSV_FLAG_NONE;
	dsvDesc.Texture2D.MipSlice = 0;
	mDSView = new DX12DepthStencilView(dsvDesc, mResource);

	// create shader view
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format                        = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDesc.ViewDimension                 = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Shader4ComponentMapping       = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Texture2D.MostDetailedMip     = 0;
	srvDesc.Texture2D.MipLevels           = 1;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	srvDesc.Texture2D.PlaneSlice          = 0;
	SetSRView(new DX12ShaderResourceView(srvDesc, mResource));

}

void DX12DepthStencil::SetWritable() {
	if (mResource->GetState() != D3D12_RESOURCE_STATE_DEPTH_WRITE) {
		mResource->Transition(D3D12_RESOURCE_STATE_DEPTH_WRITE);
	}
}

void DX12DepthStencil::SetReadable() {
	if (mResource->GetState() != D3D12_RESOURCE_STATE_GENERIC_READ) {
		mResource->Transition(D3D12_RESOURCE_STATE_GENERIC_READ);
	}
}

void DX12DepthStencil::Clear(const D3D12_CLEAR_VALUE& value) {
	SetWritable();
	GDX12Device->GetExecutor()->GetCommandList()->ClearDepthStencilView(mDSView->GetCPUHandle(), 
		D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, value.DepthStencil.Depth, value.DepthStencil.Stencil, 0, nullptr);
}


// DX12RenderTarget
DX12RenderTarget::DX12RenderTarget(DX12Resource* resource) {

	mBlendDesc = {
		FALSE, FALSE,
		D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
		D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
		D3D12_LOGIC_OP_NOOP,
		D3D12_COLOR_WRITE_ENABLE_ALL,
	};
	// int texture info
	InitWithResourceDesc(resource->GetDesc());
	
	// attach
	mResource = resource;

	// create render target view
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format               = Format;
	rtvDesc.ViewDimension        = D3D12_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice   = 0;
	rtvDesc.Texture2D.PlaneSlice = 0;
	mRTView = new DX12RenderTargetView(rtvDesc, mResource);

	// create shader resource view
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format                        = Format;
	srvDesc.ViewDimension                 = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Shader4ComponentMapping       = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Texture2D.MostDetailedMip     = 0;
	srvDesc.Texture2D.MipLevels           = 1;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	srvDesc.Texture2D.PlaneSlice          = 0;
	SetSRView(new DX12ShaderResourceView(srvDesc, mResource));

	ResetBlendState();
}


DX12RenderTarget::DX12RenderTarget(uint32_t width, uint32_t height, DXGI_FORMAT format) :
	DX12Texture()  {
	D3D12_RESOURCE_DESC dsDesc;
	dsDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	dsDesc.Alignment = 0;
	dsDesc.Width = width;
	dsDesc.Height = height;
	dsDesc.DepthOrArraySize = 1;
	dsDesc.MipLevels = 1;
	dsDesc.Format = format;
	dsDesc.SampleDesc.Count = 1;
	dsDesc.SampleDesc.Quality = 0;
	dsDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	dsDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

	// int texture info
	InitWithResourceDesc(dsDesc);

	// create resource
	mResource = new DX12Resource(D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_COMMON, dsDesc);
	
	// create render target view
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = format;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;
	rtvDesc.Texture2D.PlaneSlice = 0;
	mRTView = new DX12RenderTargetView(rtvDesc, mResource);

	// create shader view
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;
	srvDesc.Texture2D.PlaneSlice = 0;
	SetSRView(new DX12ShaderResourceView(srvDesc, mResource));
}


void DX12RenderTarget::SetReadable() {
	if (mResource->GetState() != D3D12_RESOURCE_STATE_GENERIC_READ) {
		mResource->Transition(D3D12_RESOURCE_STATE_GENERIC_READ);
	}
}

void DX12RenderTarget::SetWritable() {
	if (mResource->GetState() != D3D12_RESOURCE_STATE_RENDER_TARGET) {
		mResource->Transition(D3D12_RESOURCE_STATE_RENDER_TARGET);
	}
}

void DX12RenderTarget::SetPresent() {
	if (mResource->GetState() != D3D12_RESOURCE_STATE_PRESENT) {
		mResource->Transition(D3D12_RESOURCE_STATE_PRESENT);
	}
}

void DX12RenderTarget::Clear(const D3D12_CLEAR_VALUE& value) {
	SetWritable();
	GDX12Device->GetExecutor()->GetCommandList()->ClearRenderTargetView(mRTView->GetCPUHandle(), value.Color, 0, nullptr);
}



}