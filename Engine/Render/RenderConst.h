#pragma once

namespace z {

enum EFVFormat {
	FVF_XYZ     = 0x01,
	FVF_NORMAL,
	FVF_TANGENT,
	FVF_UV0,
	FVF_UV1,

	FVF_MAX     = FVF_UV1
};

const int MAX_FVF_NUM = 10;

inline int GetFVFSize(EFVFormat format) {
	switch (format) {
	case FVF_XYZ:
	case FVF_NORMAL:
	case FVF_TANGENT:
		return 12;
	case FVF_UV0:
	case FVF_UV1:
		return 8;
	}
	return 0;
}

enum ERenderSet {

};

}