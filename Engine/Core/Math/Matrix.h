#pragma once

#include "Vector.h"

namespace z {

template<typename T>
class TMatrix4 {
	using TVector = TVector4<T>;
public:
	TMatrix4() {}
	TMatrix4(TVector _x, TVector _y, TVector _z, TVector _w) : x(_x), y(_y), z(_z), w(_w) { }
	TMatrix4(const TMatrix4& m) { *this = m; }
	TMatrix4(Zero) : TMatrix4(0, 0, 0, 0) { }
	TMatrix4(Identity) : TMatrix4(
		TVector(1, 0, 0, 0), TVector(0, 1, 0, 0), TVector(0, 0, 1, 0), TVector(0, 0, 0, 1)
	){ 
		Log<LINFO>("here");
	}

	TVector operator* (TVector v) const {
		T fX = (t[0][0] * v[0]) + (t[1][0] * v[1]) + (t[2][0] * v[2]) + (t[3][0] * v[3]);
		T fY = (t[0][1] * v[0]) + (t[1][1] * v[1]) + (t[2][1] * v[2]) + (t[3][1] * v[3]);
		T fZ = (t[0][2] * v[0]) + (t[1][2] * v[1]) + (t[2][2] * v[2]) + (t[3][2] * v[3]);
		T fW = (t[0][3] * v[0]) + (t[1][3] * v[1]) + (t[2][3] * v[2]) + (t[3][3] * v[3]);
		return TVector(fx, fy, fz, fw);
	}

	TVector operator* (const TMatrix4& M2) const {
		TMatrix4 r;
		// Cache the invariants in registers
		T x = m[0][0];
		T y = m[0][1];
		T z = m[0][2];
		T w = m[0][3];
		// Perform the operation on the first row
		r.m[0][0] = (M2.m[0][0] * x) + (M2.m[1][0] * y) + (M2.m[2][0] * z) + (M2.m[3][0] * w);
		r.m[0][1] = (M2.m[0][1] * x) + (M2.m[1][1] * y) + (M2.m[2][1] * z) + (M2.m[3][1] * w);
		r.m[0][2] = (M2.m[0][2] * x) + (M2.m[1][2] * y) + (M2.m[2][2] * z) + (M2.m[3][2] * w);
		r.m[0][3] = (M2.m[0][3] * x) + (M2.m[1][3] * y) + (M2.m[2][3] * z) + (M2.m[3][3] * w);
		// Repeat for all the other rows
		x = m[1][0];
		y = m[1][1];
		z = m[1][2];
		w = m[1][3];
		r.m[1][0] = (M2.m[0][0] * x) + (M2.m[1][0] * y) + (M2.m[2][0] * z) + (M2.m[3][0] * w);
		r.m[1][1] = (M2.m[0][1] * x) + (M2.m[1][1] * y) + (M2.m[2][1] * z) + (M2.m[3][1] * w);
		r.m[1][2] = (M2.m[0][2] * x) + (M2.m[1][2] * y) + (M2.m[2][2] * z) + (M2.m[3][2] * w);
		r.m[1][3] = (M2.m[0][3] * x) + (M2.m[1][3] * y) + (M2.m[2][3] * z) + (M2.m[3][3] * w);
		x = m[2][0];
		y = m[2][1];
		z = m[2][2];
		w = m[2][3];
		r.m[2][0] = (M2.m[0][0] * x) + (M2.m[1][0] * y) + (M2.m[2][0] * z) + (M2.m[3][0] * w);
		r.m[2][1] = (M2.m[0][1] * x) + (M2.m[1][1] * y) + (M2.m[2][1] * z) + (M2.m[3][1] * w);
		r.m[2][2] = (M2.m[0][2] * x) + (M2.m[1][2] * y) + (M2.m[2][2] * z) + (M2.m[3][2] * w);
		r.m[2][3] = (M2.m[0][3] * x) + (M2.m[1][3] * y) + (M2.m[2][3] * z) + (M2.m[3][3] * w);
		x = m[3][0];
		y = m[3][1];
		z = m[3][2];
		w = m[3][3];
		r.m[3][0] = (M2.m[0][0] * x) + (M2.m[1][0] * y) + (M2.m[2][0] * z) + (M2.m[3][0] * w);
		r.m[3][1] = (M2.m[0][1] * x) + (M2.m[1][1] * y) + (M2.m[2][1] * z) + (M2.m[3][1] * w);
		r.m[3][2] = (M2.m[0][2] * x) + (M2.m[1][2] * y) + (M2.m[2][2] * z) + (M2.m[3][2] * w);
		r.m[3][3] = (M2.m[0][3] * x) + (M2.m[1][3] * y) + (M2.m[2][3] * z) + (M2.m[3][3] * w);
		return r;
	}

	union {
		struct { TVector x, y, z, w; };
		TVector t[4];
	};
};

typedef TMatrix4<float> Matrix4;


}