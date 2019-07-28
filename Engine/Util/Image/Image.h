#pragma once

#include <RHI/RHIConst.h>
#include <Core/CoreHeader.h>

namespace z {

class Image {
public:
	static Image* Load(std::string path);
	~Image();

	int GetWidth() {
		return mWidth;
	}

	int GetHeight() {
		return mHeight;
	}

	ERHIPixelFormat GetFormat() {
		return mFormat;
	}

	const uint8_t* GetData() {
		return mData;
	}

private:
	Image(ERHIPixelFormat, int, int, const uint8_t*);
	
	const uint8_t* mData;
	int mWidth;
	int mHeight;
	ERHIPixelFormat mFormat;

};



}