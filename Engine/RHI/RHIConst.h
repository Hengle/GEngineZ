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

struct ScreenRenderRect {
	float topLeftX;
	float topLeftY;
	float width;
	float height;
};


// Texture...
enum ERHISamplerFitler {
	SAMPLER_FILTER_UNKNOWN = 0,
	SAMPLER_FILTER_POINT,
	SAMPLER_FILTER_LINEAR,
	SAMPLER_FILTER_ANISOTROPIC
};

enum ERHISamplerAddressMode {
	SAMPLER_ADDRESS_MODE_UNKNOWN = 0,
	SAMPLER_ADDRESS_MODE_WRAP,
	SAMPLER_ADDRESS_MODE_MIRROR,
	SAMPLER_ADDRESS_MODE_CLAMP,
	SAMPLER_ADDRESS_MODE_BORDER,
	SAMPLER_MODE_MIRROR_ONCE
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

struct RHISamplerDesc {
	ERHISamplerFitler minFilter;
	ERHISamplerFitler maxFilter;
	ERHISamplerFitler mipFilter;
	ERHISamplerAddressMode addressU;
	ERHISamplerAddressMode addressV;
	ERHISamplerAddressMode addressW;
};

struct RHITextureDesc {
	uint32_t sizeX;
	uint32_t sizeY;
	uint32_t sizeZ;
	uint32_t flags;
	RHISamplerDesc samplerDesc;
	ERHIPixelFormat format;
	ERHITexDimension dimension;
	uint8_t numMips;
};

struct RHITextureData {
	uint32_t datalen;
	uint8_t* data;
};








struct BlendState {

};


}