#pragma once

namespace z {

enum EFVFormat {
	FVF_XYZ    = 0x01,
	FVF_UV0    = 0x02,
	FVF_UV1    = 0x04,
	FVF_NORMAL = 0x08,

	FVF_MAX = FVF_NORMAL
};

inline int GetFVFSize(EFVFormat format) {
	switch (format) {
	case FVF_XYZ:
	case FVF_NORMAL:
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