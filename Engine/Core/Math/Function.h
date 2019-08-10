#pragma once

#include "Vector.h"
#include "Matrix.h"

namespace z {


// Vector3 Functions
template <typename T>
inline T Dot(TVector3<T> t1, TVector3<T> t2) {
	return t1.x * t2.x + t1.y * t2.y + t1.z * t2.z;
}

template <typename T>
inline TVector3<T> Cross(TVector3<T> t1, TVector3<T> t2) {
	return TVector3<T> {
		(t1.y * t2.z) - (t1.z * t2.y), (t1.z * t2.x) - (t1.x * t2.z), (t1.x * t2.y) - (t1.y * t2.x)
	};
}

template <typename T>
inline TVector3<T> Normalize(TVector3<T> v) {
	T l = std::sqrt(Dot(v, v));
	return v / l;
}


// Matrix4 Funcions
template <typename T>
inline TMatrix4<T> Transpose(TMatrix4<T> m) {
	return TMatrix4<T>{
		m[0][0], m[0][1], m[0][2], m[0][3],
		m[1][0], m[1][1], m[1][2], m[1][3],
		m[2][0], m[2][1], m[2][2], m[2][3],
		m[3][0], m[3][1], m[3][2], m[3][3]
	};

}

inline Matrix4 MatrixLookAtLH(Vector3F pos, Vector3F target, Vector3F up) {
	Vector3 forward = Normalize(target - pos);
	Vector3 right = Normalize(Cross(up, forward));
	up = Normalize(Cross(forward, right));
	
	Matrix4 m = Matrix4(
		{ right, -Dot(right, pos) },
		{ up, -Dot(up, pos) },
		{ forward, -Dot(forward, pos) },
		{ 0, 0, 0, 1 }
	);

	return Matrix4(
		{ right, -Dot(right, pos) },
		{ up, -Dot(up, pos) },
		{ forward, -Dot(forward, pos) },
		{ 0, 0, 0, 1 }
	);
}


inline Matrix4 PerspectiveFovLH(float fov, float aspect, float nearZ, float farZ) {
	float sinv = std::sin(fov / 2);
	float cosv = std::cos(fov / 2);

	float tanv = sinv / cosv;
	float range = farZ / (farZ - nearZ);

	return Matrix4(
		1 / tanv / aspect, 0, 0, 0,
		0, 1 / tanv, 0, 0,
		0, 0, range, -range * nearZ,
		0, 0, 1, 0
	);
}

}