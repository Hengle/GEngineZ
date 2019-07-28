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
		{PIXEL_FORMAT_R32_FLOAT         , DXGI_FORMAT_R32_FLOAT},
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

inline const D3D12_RESOURCE_DESC FromRHITextureDesc(const RHITextureDesc &rhi_desc) {
	D3D12_RESOURCE_DESC ret;
	if (rhi_desc.sizeY == 0 && rhi_desc.sizeZ == 0) {
		CHECK(0, "not support");
	} else if (rhi_desc.sizeZ == 0) {
		ret = CD3DX12_RESOURCE_DESC::Tex2D(
			FromRHIFormat(rhi_desc.format),
			rhi_desc.sizeX, rhi_desc.sizeY, 1, rhi_desc.numMips
		);
	} else {
		CHECK(0, "not support");
	}
	return ret;
}

}
