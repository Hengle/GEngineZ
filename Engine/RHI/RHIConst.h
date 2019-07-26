#pragma once

#include <Core/CoreHeader.h>

namespace z {

enum DeviceFlag {
	
	EDevice_DF_EnableDebug = 1
	// 
	//EDEVICE_CF_
};

// Texture
enum PixelFormat {
	EPixelFormat_Invaild = 0,
	EPixelFormat_R8G8B8A8 = 0,

};

struct TexClearValue {
	TexClearValue() {
		value.color[0] = 0.0f;
		value.color[1] = 0.0f;
		value.color[2] = 0.0f;
		value.color[3] = 0.0f;
	}

	TexClearValue(float val0, float val1, float val2, float val3) {
		value.color[0] = val0;
		value.color[1] = val1;
		value.color[2] = val2;
		value.color[3] = val3;
	}

	TexClearValue(float depth, uint32_t stencil) {
		value.ds.depth = depth;
		value.ds.stencil = stencil;
	}

	union {
		float color[4];
		struct {
			float depth;
			uint32_t stencil;
		} ds;
	} value;
};


struct RHITextureDesc {
	uint32_t sizeX;
	uint32_t sizeY;
	uint32_t sizeZ;
	uint32_t numMips;
	uint32_t numSamples;
	uint32_t pixelFormat;
	uint32_t flags;
	TexClearValue clearv;
};


// Shader
enum EShaderType {
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

enum EPixelFormat{
	VERTEX_FORMAT_INVALID = 0,
	VERTEX_FORAMT_R32_FLAOT,
	VERTEX_FORAMT_R32G12_FLAOT,
	VERTEX_FORMAT_R32G32B32_FLOAT,
	VERTEX_FORMAT_R32G32B32A32_FLOAT,

	VERTEX_FORMAT_MAX
};

enum EUniformLayoutFlag {
	UNIFORM_LAYOUT_INVALID = 0,
	UNIFORM_LAYOUT_CONSTANT_BUFFER = 1,
};




struct BlendState {

};


}