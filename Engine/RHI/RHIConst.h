#pragma once

#include <Core/CoreHeader.h>

namespace z {

enum DeviceFlag {
	EDevice_DF_EnableDebug = 1
	// 
	//EDEVICE_CF_
};


enum ERHIShaderStage {
	SHADER_STAGE_VERTEX,
	SHADER_STAGE_PIXEL,
};


enum ERHIPixelFormat {
	PF_INVALID = 0,
	PF_R32,
	PF_R32G32,
	PF_R32G32B32,
	PF_R32G32B32A32,
	PF_R8G8B8A8,
	PF_D24S8
};


enum ERHITexDimension {
	TEX_DIMENSION_UNKNOWN = 0,
	TEX_DIMENSION_1D,
	TEX_DIMENSION_2D,
	TEX_DIMENSION_3D,
	TEX_DIMENSION_CUBE,
	TEX_DIMENSION_1D_ARRAY,
	TEX_DIMENSION_2D_ARRAY,
	TEX_DIMENSION_3D_ARRAY,
};


enum ERHISamplerFlag {
	SAMPLER_FILTER_LINEAR		= 0x0001,	// default 0x00ff
	SAMPLER_FILTER_POINT		= 0x0002,
	SAMPLER_FILTER_ANISOTROPIC	= 0x0004,
	SAMPLER_ADDRESS_WRAP		= 0x0100,	// default 0xff00
	SAMPLER_ADDRESS_MIRROR		= 0x0200,
	SAMPLER_ADDRESS_CLAMP		= 0x0400,
	SAMPLER_ADDRESS_BORDER		= 0x0800,

};

enum ERHIRenderState {
	// 2 bit, fill mode
	RS_FILL_WIREFRAME = 0x01 << 0,
	RS_FILL_SOLID	  = 0x02 << 0,
	// 2 bit, cull mode
	RS_CULL_NONE	  = 0x01 << 2,
	RS_CULL_FRONT	  = 0x02 << 2,
	RS_CULL_BACK	  = 0x03 << 2,
	// blend

	// stencil

	// depth
};

enum ERHIInputSemantic {
	SEMANTIC_POSITION	= 0x01,
	SEMANTIC_NORMAL,
	SEMANTIC_TANGENT,
	SEMANTIC_UV0,
	SEMANTIC_UV1,

	SEMANTIC_MAX = FVF_UV1
};

const int MAX_INPUT_SEMANTIC = 10;




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


struct ScreenRenderRect {
	float topLeftX;
	float topLeftY;
	float width;
	float height;
};

struct RHIInputDesc {
	std::string SemanticName;
	uint32_t SemanticIndex;
	ERHIPixelFormat Format;
};



struct RHITextureDesc {
	uint32_t sizeX;
	uint32_t sizeY;
	uint32_t sizeZ;
	uint32_t flags;
	ERHIPixelFormat format;
	ERHITexDimension dimension;
	uint8_t numMips;
};






struct BlendState {

};


}