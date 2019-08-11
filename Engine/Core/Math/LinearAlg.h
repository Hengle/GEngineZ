#pragma once

#include "Vector.h"
#include "Matrix.h"

namespace z {
namespace math {


// Vector
template <typename T>
inline T Dot(TVector3<T> t1, TVector3<T> t2) {
	return t1.x * t2.x + t1.y * t2.y + t1.z * t2.z;
}

template <typename T>
inline TVector3<T> Cross(TVector3<T> t1, TVector3<T> t2) {
	return TVector3<T> {
		(t1.y* t2.z) - (t1.z * t2.y), (t1.z* t2.x) - (t1.x * t2.z), (t1.x* t2.y) - (t1.y * t2.x)
	};
}

template <typename T>
inline TVector3<T> Normalize(TVector3<T> v) {
	T l = std::sqrt(Dot(v, v));
	return v / l;
}


// Matrix
template <typename T>
inline TMatrix4<T> Transpose(TMatrix4<T> m) {
	return TMatrix4<T>{
		m[0][0], m[0][1], m[0][2], m[0][3],
			m[1][0], m[1][1], m[1][2], m[1][3],
			m[2][0], m[2][1], m[2][2], m[2][3],
			m[3][0], m[3][1], m[3][2], m[3][3]
	};

}

/*
RotationAxis
NxNx(1-Cos)+Cos   NxNy(1-Cos)+NzSin NxNz(1-Cos)-NySin
NxNy(1-Cos)-NzSin NyNy(1-Cos)+Cos   NyNz(1-Cos)+NxSin
NxNz(1-Cos)+Nysin NxNy(1-Cos)-NxCos NzNz(1-Cos)+Cos
*/
inline Matrix4F MatrixRotationAxis(Vector3F axis, float radian) {
	axis = Normalize(axis);
	float sinv = std::sin(radian);
	float cosv = std::cos(radian);

	Vector3F V0 = Vector3F(1.0f-cosv) * Vector3F(axis.y, axis.z, axis.x) * Vector3F(axis.z, axis.x, axis.y);

	Vector3F R0 = Vector3F(1.0f-cosv) * axis * axis + Vector3F(cosv);
	Vector3F R1 = V0 + Vector3F(sinv) * axis;
	Vector3F R2 = V0 - Vector3F(sinv) * axis;

	return Matrix4F{
		{R0.x, R2.z, R1.y, 0},
		{R1.z, R0.y, R2.x, 0},
		{R2.y, R1.x, R0.z, 0},
		{0,    0,    0,    1}
	};

}


}
}