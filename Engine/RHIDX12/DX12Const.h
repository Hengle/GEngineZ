#pragma once

#include <unordered_map>
#include <RHI/RHIConst.h>

namespace z {
struct RHIFormatInfo {
	DXGI_FORMAT format;
	uint32_t size;
};

const static RHIFormatInfo PixelFomat[VERTEX_FORMAT_MAX] = {
	{DXGI_FORMAT_UNKNOWN, 0},	// VERTEX_FORMAT_INVALID
	{DXGI_FORMAT_R32_FLOAT, 4},	// VERTEX_FORAMT_R32_FLAOT
	{DXGI_FORMAT_R32G32_FLOAT, 8},
	{DXGI_FORMAT_R32G32B32_FLOAT, 12},
	{DXGI_FORMAT_R32G32B32A32_FLOAT, 16},
};
}