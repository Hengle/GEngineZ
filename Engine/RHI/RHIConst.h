#pragma once

#include <Core/CoreHeader.h>

namespace z {

enum DeviceFlag {

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
	PF_R16G16B16A16,
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


// render state
enum ERHIRenderState {
	// fill mode, 2 bit
	RS_FILL_WIREFRAME      = 0x01,
	RS_FILL_SOLID          = 0x02,
	// cull mode, 2bit
	RS_CULL_NONE           = 0x01,
	RS_CULL_FRONT          = 0x02,
	RS_CULL_BACK           = 0x03,
	// compare func, 4 bit
	RS_COMP_FUNC_NEVER     = 0x01,
	RS_COMP_FUNC_EQUAL     = 0x02,
	RS_COMP_FUNC_LESS      = 0x03,
	RS_COMP_FUNC_LEQUAL    = 0x04,
	RS_COMP_FUNC_GREATER   = 0x05,
	RS_COMP_FUNC_GEQUAL    = 0x06,
	RS_COMP_FUNC_NEQUAL    = 0x07,
	RS_COMP_FUNC_ALWAYS    = 0x08,
	// stencil op func, 4bit
	RS_STENCIL_OP_KEEP     = 0x01,
	RS_STENCIL_OP_ZERO     = 0x02,
	RS_STENCIL_OP_REPLACE  = 0x03,
	RS_STENCIL_OP_INCR_SAT = 0x04,
	RS_STENCIL_OP_DECR_SAT = 0x05,
	RS_STENCIL_OP_INVERT   = 0x06,
	RS_STENCIL_OP_INCR     = 0x07,
	RS_STENCIL_OP_DECR     = 0x08,
};


// blend state is bind to render target
enum ERHIBlendOperation {
	BLEND_OP_ADD = 1,
	BLEND_OP_SUBTRACT,
	BLEND_OP_REV_SUBTRACT,
	BLEND_OP_MIN,
	BLEND_OP_MAX,
};

enum ERHIBlendFactor {
	BLEND_FACTOR_ZERO = 1,
	BLEND_FACTOR_ONE,
	BLEND_FACTOR_SRC_COLOR,
	BLEND_FACTOR_INV_SRC_COLOR,
	BLEND_FACTOR_SRC_ALPHA,
	BLEND_FACTOR_INV_SRC_ALPHA,
	BLEND_FACTOR_DEST_ALPHA,
	BLEND_FACTOR_INV_DEST_ALPHA,
	BLEND_FACTOR_DEST_COLOR,
	BLEND_FACTOR_INV_DEST_COLOR,
	BLEND_FACTOR_SRC_ALPHA_SAT,
	BLEND_FACTOR_BLEND_FACTOR,
	BLEND_FACTOR_INV_BLEND_FACTOR,
	BLEND_FACTOR_SRC1_COLOR,
	BLEND_FACTOR_INV_SRC1_COLOR,
	BLEND_FACTOR_SRC1_ALPHA,
	BLEND_FACTOR_INV_SRC1_ALPHA
};

enum ERHIInputSemantic {
	SEMANTIC_POSITION	= 0x01,
	SEMANTIC_NORMAL,
	SEMANTIC_TANGENT,
	SEMANTIC_BINORMAL,
	SEMANTIC_UV0,
	SEMANTIC_UV1,
	SEMANTIC_COLOR,
	SEMANTIC_POSITION2D,

	SEMANTIC_MAX = SEMANTIC_POSITION2D + 1
};

struct RHIRenderState {
	union {
		struct {
			uint8_t
				// switch
				EnableDepthTest      : 1,	// 1
				EnableDepthWrite     : 1,	// 2
				EnableStencil        : 1,	// 3
								     : 1,	// 4
				// Rasterizer
				FillMode             : 2,	// 6
				CullMode             : 2,	// 8
				// DepthStencil
				DepthCompFunc        : 4,	// 12
				StencilReadMask      : 8,	// 20
				StencilWriteMask     : 8,	// 28
				StencilFrontPassOp   : 4,	// 32
				StencilForntFailOp   : 4,	// 36
				StencilFrontZFailOp  : 4,	// 40
				StencilFrontCompFunc : 4,	// 44
				StencilBackPassOp    : 4,	// 48
				StencilBackFailOp    : 4,	// 52
				StencilBackZFailOp   : 4,	// 56
				StencilBackCompFunc  : 4	// 60
				;
		};
		uint64_t Value;
	};
};


struct RHIRenderRect {
	float TopLeftX;
	float TopLeftY;
	float Width;
	float Height;
	float MinDepth;
	float MaxDepth;
};


struct RHIScissorRect {
	int Left;
	int Top;
	int Right;
	int Bottom;
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


struct RHIBlendState {
	bool Enable;
	ERHIBlendFactor SrcBlend;
	ERHIBlendFactor DestBlend;
	ERHIBlendOperation BlendOp;
	ERHIBlendFactor SrcBlendAlpha;
	ERHIBlendFactor DestBlendAlpha;
	ERHIBlendOperation BlendOpAlpha;
};


struct RHITextureDesc {
	uint8_t NumMips;
	uint32_t SizeX;
	uint32_t SizeY;
	uint32_t SizeZ;
	uint32_t Flags;
	ERHIPixelFormat Format;
	ERHITexDimension Dimension;
};




}