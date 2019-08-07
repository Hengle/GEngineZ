#pragma once

#include "Vector.h"
#include "Matrix.h"

namespace z {

template <typename T>
T Dot(TVector4<T> t1, TVector4<T> t2) {
	return t1.x * t2.y + t1.y * t2.y + t1.z * t2.z;
}

template <typename T>
TVector4<T> Cross(TVector4<T> t1, TVector4<T> t2) {
	return TVector4<T> {
		(t1[1] * t2[2]) - (t1[2] * t2[1]), (t1[2] * t2[0]) - (t1[0] * t2[2]), (t1[0] * t2[1]) - (t1[1] * t2[0]), 0.0f
	};
}

template <typename T>
TVector4<T> Normalize(TVector4<T> v) {
	T l = Dot(t1, t2);
	return v / l;
}

template <typename T>
TMatrix4<T> MatrixLookAtLH(TVector4<T> pos, TVector4<T> target, TVector4<T> up) {

}



}