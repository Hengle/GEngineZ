#pragma once
#include "RHIConst.h"

namespace z {

inline int GetRHIPixelFormatSize(ERHIPixelFormat format) {
	switch (format) {
	case PF_R32:
	case PF_D24S8:
	case PF_R8G8B8A8:
		return 4;
	case PF_R32G32:
		return 8;
	case PF_R32G32B32:
		return 12;
	case PF_R32G32B32A32:
		return 16;
	}
	return 0;
}

}