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
		{PIXEL_FORMAT_R32G32B32_FLOAT   ,DXGI_FORMAT_R32G32B32_FLOAT},
		{PIXEL_FORMAT_R32G32B32A32_FLOAT,DXGI_FORMAT_R32G32B32A32_FLOAT},
		{PIXEL_FORMAT_R8G8B8A8_UNORM    ,DXGI_FORMAT_R8G8B8A8_UNORM},
		{PIXEL_FORMAT_D24_UNORM_S8_UINT ,DXGI_FORMAT_D24_UNORM_S8_UINT}
	};
	return formatMap[format];
}

inline uint16_t GetPixelSize(DXGI_FORMAT format) {
	switch (format) {
	case PIXEL_FORMAT_R32_FLOAT:
	case PIXEL_FORMAT_D24_UNORM_S8_UINT:
	case PIXEL_FORMAT_R8G8B8A8_UNORM:
		return 4;
	case PIXEL_FORMAT_R32G32_FLOAT:
		return 8;
	case PIXEL_FORMAT_R32G32B32_FLOAT:
		return 12;
	case PIXEL_FORMAT_R32G32B32A32_FLOAT:
		return 16;
	}
	return 0;
}

}
