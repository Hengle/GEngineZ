#pragma once

#include <Core/CoreHeader.h>

namespace z {

enum DeviceFlag {
	
	EDevice_DF_EnableDebug = 1
	// 
	//EDEVICE_CF_
};

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

}