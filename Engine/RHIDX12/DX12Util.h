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

#define DECLARE_PF_MAP(Define)\
	Define(PF_INVALID     , DXGI_FORMAT_UNKNOWN),           \
	Define(PF_R32         , DXGI_FORMAT_R32_FLOAT),         \
	Define(PF_R32G32      , DXGI_FORMAT_R32G32_FLOAT),      \
	Define(PF_R32G32B32   , DXGI_FORMAT_R32G32B32_FLOAT),   \
	Define(PF_R32G32B32A32, DXGI_FORMAT_R32G32B32A32_FLOAT),\
	Define(PF_R8G8B8A8    , DXGI_FORMAT_R8G8B8A8_UNORM),    \
	Define(PF_D24S8       , DXGI_FORMAT_D24_UNORM_S8_UINT)


inline DXGI_FORMAT FromRHIFormat(ERHIPixelFormat format) {
	static std::unordered_map<ERHIPixelFormat, DXGI_FORMAT> formatMap = {
#define RHI_TO_DXGI(rhi, dxgi) {rhi, dxgi}
		DECLARE_PF_MAP(RHI_TO_DXGI)
#undef RHI_TO_DXGI
	};
	return formatMap[format];
}

inline ERHIPixelFormat FromDXGIFormat(DXGI_FORMAT format) {
	static std::unordered_map<DXGI_FORMAT, ERHIPixelFormat> formatMap = {
#define DXGGI_TO_RHI(rhi, dxgi) {dxgi, rhi}
		DECLARE_PF_MAP(DXGGI_TO_RHI)
#undef RHI_TO_DXGI
	};
	return formatMap[format];
}

#undef DECLARE_PF_MAP

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

//inline const D3D12_SAMPLER_DESC FromRHISamplerDesc(const RHISamplerDesc& desc) {
//	auto static DecideAddressMode = [](ERHISamplerAddressMode mode) -> D3D12_TEXTURE_ADDRESS_MODE {
//		if (mode == SAMPLER_ADDRESS_MODE_WRAP) 
//			return D3D12_TEXTURE_ADDRESS_MODE_WRAP;
//		if (mode == SAMPLER_ADDRESS_MODE_MIRROR) 
//			return D3D12_TEXTURE_ADDRESS_MODE_MIRROR;
//		if (mode == SAMPLER_ADDRESS_MODE_CLAMP) 
//			return D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
//		if (mode == SAMPLER_ADDRESS_MODE_BORDER) 
//			return D3D12_TEXTURE_ADDRESS_MODE_BORDER;
//		if (mode == SAMPLER_MODE_MIRROR_ONCE) 
//			return D3D12_TEXTURE_ADDRESS_MODE_MIRROR_ONCE;
//		return D3D12_TEXTURE_ADDRESS_MODE_WRAP;
//	};
//		
//	auto static DecideFilter = [](ERHISamplerFitler minf, ERHISamplerFitler maxf, ERHISamplerFitler mipf) -> D3D12_FILTER {
//#define DECIDE(m0, m1, m2, mr)	if (minf == m0 && maxf == m1 && mipf == m2) return mr;
//		DECIDE(SAMPLER_FILTER_POINT  , SAMPLER_FILTER_POINT  , SAMPLER_FILTER_POINT  , D3D12_FILTER_MIN_MAG_MIP_POINT);
//		DECIDE(SAMPLER_FILTER_POINT  , SAMPLER_FILTER_POINT  , SAMPLER_FILTER_LINEAR , D3D12_FILTER_MIN_MAG_MIP_POINT);
//		DECIDE(SAMPLER_FILTER_POINT  , SAMPLER_FILTER_LINEAR , SAMPLER_FILTER_POINT  , D3D12_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT);
//		DECIDE(SAMPLER_FILTER_POINT  , SAMPLER_FILTER_LINEAR , SAMPLER_FILTER_LINEAR , D3D12_FILTER_MIN_POINT_MAG_MIP_LINEAR);
//		DECIDE(SAMPLER_FILTER_LINEAR , SAMPLER_FILTER_POINT  , SAMPLER_FILTER_POINT  , D3D12_FILTER_MIN_LINEAR_MAG_MIP_POINT);
//		DECIDE(SAMPLER_FILTER_LINEAR , SAMPLER_FILTER_POINT  , SAMPLER_FILTER_LINEAR , D3D12_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR);
//		DECIDE(SAMPLER_FILTER_LINEAR , SAMPLER_FILTER_LINEAR , SAMPLER_FILTER_POINT  , D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT);
//		DECIDE(SAMPLER_FILTER_LINEAR , SAMPLER_FILTER_LINEAR , SAMPLER_FILTER_LINEAR , D3D12_FILTER_MIN_MAG_MIP_LINEAR);
//		DECIDE(SAMPLER_FILTER_ANISOTROPIC, SAMPLER_FILTER_ANISOTROPIC, SAMPLER_FILTER_ANISOTROPIC, D3D12_FILTER_ANISOTROPIC);
//#undef DECIDE
//		Log<LWARN>("sampler filter not supported");
//		return D3D12_FILTER_MIN_MAG_MIP_LINEAR;
//	};
//
//
//	D3D12_SAMPLER_DESC samplerDesc{};
//	samplerDesc.Filter         = DecideFilter(desc.minFilter, desc.maxFilter, desc.mipFilter);
//	samplerDesc.AddressU       = DecideAddressMode(desc.addressU);
//	samplerDesc.AddressV       = DecideAddressMode(desc.addressV);
//	samplerDesc.AddressW       = DecideAddressMode(desc.addressW);
//	samplerDesc.MipLODBias     = 0;
//	samplerDesc.MaxAnisotropy  = 1;
//	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
//	samplerDesc.BorderColor[4] = { 0.f };
//	samplerDesc.MinLOD         = 0;
//	samplerDesc.MaxLOD         = D3D12_FLOAT32_MAX;
//	return samplerDesc;
//}



}
