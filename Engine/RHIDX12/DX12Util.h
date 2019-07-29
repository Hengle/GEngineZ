#pragma once

#include "DX12Const.h"
#include <RHI/RHIConst.h>

namespace z {

inline D3D12_CLEAR_VALUE FromRHIClearVale(const RHIClearValue& rv) {
	D3D12_CLEAR_VALUE v;
	v.Format = DXGI_FORMAT_UNKNOWN;
	memcpy(v.Color, rv.color, sizeof(float) * 4);
	return v;
}

inline DXGI_FORMAT FromRHIFormat(ERHIPixelFormat format) {
	static std::unordered_map<ERHIPixelFormat, DXGI_FORMAT> formatMap = {
		{PIXEL_FORMAT_INVALID           , DXGI_FORMAT_UNKNOWN},
		{PIXEL_FORMAT_R32_F         , DXGI_FORMAT_R32_FLOAT},
		{PIXEL_FORMAT_R32G32_FLOAT      , DXGI_FORMAT_R32G32_FLOAT},
		{PIXEL_FORMAT_R32G32B32_FLOAT   , DXGI_FORMAT_R32G32B32_FLOAT},
		{PIXEL_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_R32G32B32A32_FLOAT},
		{PIXEL_FORMAT_R8G8B8A8_UNORM    , DXGI_FORMAT_R8G8B8A8_UNORM},
		{PIXEL_FORMAT_D24_UNORM_S8_UINT , DXGI_FORMAT_D24_UNORM_S8_UINT}
	};
	return formatMap[format];
}

inline uint16_t GetPixelSize(DXGI_FORMAT format) {
	switch (format) {
	case DXGI_FORMAT_R32_FLOAT:
	case DXGI_FORMAT_D24_UNORM_S8_UINT:
	case DXGI_FORMAT_R8G8B8A8_UNORM:
		return 4;
	case DXGI_FORMAT_R32G32_FLOAT:
		return 8;
	case DXGI_FORMAT_R32G32B32_FLOAT:
		return 12;
	case DXGI_FORMAT_R32G32B32A32_FLOAT:
		return 16;
	}
	return 0;
}

inline const D3D12_SAMPLER_DESC FromRHISamplerDesc(const RHISamplerDesc& desc) {
	auto static DecideAddressMode = [](ERHISamplerAddressMode mode) -> D3D12_TEXTURE_ADDRESS_MODE {
		if (mode == SAMPLER_ADDRESS_MODE_WRAP) 
			return D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		if (mode == SAMPLER_ADDRESS_MODE_MIRROR) 
			return D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
		if (mode == SAMPLER_ADDRESS_MODE_CLAMP) 
			return D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
		if (mode == SAMPLER_ADDRESS_MODE_BORDER) 
			return D3D12_TEXTURE_ADDRESS_MODE_BORDER;
		if (mode == SAMPLER_MODE_MIRROR_ONCE) 
			return D3D12_TEXTURE_ADDRESS_MODE_MIRROR_ONCE;
		return D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	};
		
	auto static DecideFilter = [](ERHISamplerFitler minf, ERHISamplerFitler maxf, ERHISamplerFitler mipf) -> D3D12_FILTER {
#define DECIDE(m0, m1, m2, mr)	if (minf == m0 && maxf == m1 && mipf == m2) return mr;
		DECIDE(SAMPLER_FILTER_POINT  , SAMPLER_FILTER_POINT  , SAMPLER_FILTER_POINT  , D3D12_FILTER_MIN_MAG_MIP_POINT);
		DECIDE(SAMPLER_FILTER_POINT  , SAMPLER_FILTER_POINT  , SAMPLER_FILTER_LINEAR , D3D12_FILTER_MIN_MAG_MIP_POINT);
		DECIDE(SAMPLER_FILTER_POINT  , SAMPLER_FILTER_LINEAR , SAMPLER_FILTER_POINT  , D3D12_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT);
		DECIDE(SAMPLER_FILTER_POINT  , SAMPLER_FILTER_LINEAR , SAMPLER_FILTER_LINEAR , D3D12_FILTER_MIN_POINT_MAG_MIP_LINEAR);
		DECIDE(SAMPLER_FILTER_LINEAR , SAMPLER_FILTER_POINT  , SAMPLER_FILTER_POINT  , D3D12_FILTER_MIN_LINEAR_MAG_MIP_POINT);
		DECIDE(SAMPLER_FILTER_LINEAR , SAMPLER_FILTER_POINT  , SAMPLER_FILTER_LINEAR , D3D12_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR);
		DECIDE(SAMPLER_FILTER_LINEAR , SAMPLER_FILTER_LINEAR , SAMPLER_FILTER_POINT  , D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT);
		DECIDE(SAMPLER_FILTER_LINEAR , SAMPLER_FILTER_LINEAR , SAMPLER_FILTER_LINEAR , D3D12_FILTER_MIN_MAG_MIP_LINEAR);
		DECIDE(SAMPLER_FILTER_ANISOTROPIC, SAMPLER_FILTER_ANISOTROPIC, SAMPLER_FILTER_ANISOTROPIC, D3D12_FILTER_ANISOTROPIC);
#undef DECIDE
		Log<LWARN>("sampler filter not supported");
		return D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	};


	D3D12_SAMPLER_DESC samplerDesc{};
	samplerDesc.Filter         = DecideFilter(desc.minFilter, desc.maxFilter, desc.mipFilter);
	samplerDesc.AddressU       = DecideAddressMode(desc.addressU);
	samplerDesc.AddressV       = DecideAddressMode(desc.addressV);
	samplerDesc.AddressW       = DecideAddressMode(desc.addressW);
	samplerDesc.MipLODBias     = 0;
	samplerDesc.MaxAnisotropy  = 1;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	samplerDesc.BorderColor[4] = { 0.f };
	samplerDesc.MinLOD         = 0;
	samplerDesc.MaxLOD         = D3D12_FLOAT32_MAX;
	return samplerDesc;
}

inline const D3D12_CULL_MODE FromRHICullMode(const ERHICullMode& cullmode) {
	static std::unordered_map<ERHICullMode, D3D12_CULL_MODE> cullmodeMap = {
		{CULL_MODE_NONE, D3D12_CULL_MODE_NONE},
		{CULL_MODE_FRONT, D3D12_CULL_MODE_FRONT},
		{CULL_MODE_BACK, D3D12_CULL_MODE_BACK},
	};
	return cullmodeMap[cullmode];
}

inline const D3D12_FILL_MODE FromRHIFillMode(const ERHIFillMode& fillmode) {
	static std::unordered_map<ERHIFillMode, D3D12_FILL_MODE> fillmodeMap = {
		{FILL_MODE_WIREFRAME, D3D12_FILL_MODE_WIREFRAME},
		{FILL_MODE_SOLID,	D3D12_FILL_MODE_SOLID},
	};
	return fillmodeMap[fillmode];
}


}
