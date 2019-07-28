#pragma once

#include <Core/CoreHeader.h>

namespace z {

enum DeviceFlag {

	EDevice_DF_EnableDebug = 1
	// 
	//EDEVICE_CF_
};


enum ERHIShaderType {
	SHADER_TYPE_UNKNOWN = 0,
	SHADER_TYPE_VERTEX,
	SHADER_TYPE_PIXEL,

	SHADER_TYPE_MAX
};


enum EVertexLaytoutFlag {
	VERTEX_LAYOUT_INVALID = 0,
	VERTEX_LAYOUT_PER_INSTANCE,
	VERTEX_LAYOUT_PER_VERTEX,
};


enum ERHIPixelFormat {
	PIXEL_FORMAT_INVALID = 0,
	PIXEL_FORMAT_R32_FLOAT,
	PIXEL_FORMAT_R32G32_FLOAT,
	PIXEL_FORMAT_R32G32B32_FLOAT,
	PIXEL_FORMAT_R32G32B32A32_FLOAT,
	PIXEL_FORMAT_R8G8B8A8_UNORM,
	PIXEL_FORMAT_D24_UNORM_S8_UINT,

	PIXEL_FORMAT_MAX
};


enum EUniformLayoutFlag {
	UNIFORM_LAYOUT_INVALID = 0,
	UNIFORM_LAYOUT_CONSTANT_BUFFER = 1,
	UNIFORM_LAYOUT_TEXTURE = 2,
};


struct RHIClearValue {
	RHIClearValue(float val0, float val1, float val2, float val3) {
		color[0] = val0;
		color[1] = val1;
		color[2] = val2;
		color[3] = val3;
	}

	RHIClearValue(float depth, uint32_t stencil) {
		depthsStencil.depth = depth;
		depthsStencil.stencil = stencil;
	}

	union {
		float color[4];
		struct {
			float depth;
			uint8_t stencil;
		} depthsStencil;
	};
};

class RHIShader;
class RHIVertexLayout;
class RHIUniformLayout;

struct RHIPipelineStateDesc {
	RHIPipelineStateDesc() :
		vs(nullptr), ps(nullptr), vlayout(nullptr), ulayout(nullptr),
		dsFormat(PIXEL_FORMAT_INVALID) {
	}

	RHIShader* vs;
	RHIShader* ps;
	RHIVertexLayout* vlayout;
	RHIUniformLayout* ulayout;
	std::vector<ERHIPixelFormat> rtsFormat;
	ERHIPixelFormat dsFormat;	
};


struct RHITextureDesc {
	uint32_t sizeX;
	uint32_t sizeY;
	uint32_t sizeZ;
	uint32_t numMips;
	uint32_t numSamples;
	ERHIPixelFormat format;
	uint32_t flags;
};










struct BlendState {

};


}