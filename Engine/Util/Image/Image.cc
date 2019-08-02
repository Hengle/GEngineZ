#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_TGA
#include <stb/stb_image.h>

namespace z {


Image* Image::Load(std::string path) {
	int x, y, comp;
	uint8_t* data = stbi_load(path.c_str(), &x, &y, &comp, 0);
	if (data == nullptr) {
		return nullptr;
	}

	ERHIPixelFormat format = PIXEL_FORMAT_INVALID;
	switch (comp) {
	case STBI_grey:
		CHECK(0, "not support");
		break;
	case STBI_grey_alpha:
		CHECK(0, "not support");
		break;
	case STBI_rgb:
		// do convert because direct x not support r8g8b8
		data = stbi__convert_format(data, comp, STBI_rgb_alpha, x, y);
		comp = STBI_rgb_alpha;
		format = PIXEL_FORMAT_R8G8B8A8_UNORM;
		break;
	case STBI_rgb_alpha:
		format = PIXEL_FORMAT_R8G8B8A8_UNORM;
		break;
	}

	Log<LDEBUG>("Load Image", path.c_str(), x, y, comp);
	return new Image(format, x, y, data);
}

Image::Image(ERHIPixelFormat format, int width, int height, const uint8_t* data) :
	mFormat(format), 
	mWidth(width),
	mHeight(height),
	mData(data) {
}

Image::~Image() {
	SAFE_DELETE(mData);
}


}

