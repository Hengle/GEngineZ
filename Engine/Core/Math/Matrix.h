#pragma once

#include "Vector.h"

namespace z {

// 
/*
// left vector multiply
[x, y, z, w'] * Matrix = [x', y', z', w']

v * world * view * proj

//column-major matrix
|a00 a10 a20 a30|
|a01 a11 a21 a31|
|a02 a12 a22 a32|
|a03 a13 a23 a33|

// world (just transform example)
|1 0 0 0|
|0 1 0 0|
|0 0 1 0|
|x y z 0|

// view
d(direction,z), r(right,x), u(up,y), p(position)
|rx ry rz 0|        |1 0 0 0|          |rx ux dx 0|
|ux uy yz 0| * V =  |0 1 0 0|   => V = |ry uy dy 0|  => T = [-p*r, -p*u, -p*d, 0]
|dx dy dz 0|        |0 0 1 0|          |rz uz dz 0|
|px py pz 1|        |0 0 0 1|          | --T---  1|

// Perspective
P(positiion), P'(projetion in near plane), aspect(w/h), f(far), n(near)
1. projection to near plane
P' = (n *px / pz, n * py / pz, n)
2. mapping H -> 2
P''y = P'y / (H / 2) = near * Py / Pz / (H / 2) = Py / Pz / tan(fov/2) 
P''x = Px / Pz / tan(fov/2) / aspect
3. keep z, [n,f]->[0,1]
P''z = a / pz + b,  => a = fn/(n-f), b= f/(f-n)

P * Proj = P''

a = aspect, t = tan(fov/2)
               | 1/(a*t) 0    0       0|
[x, y, z, 1] * | 0       1/t  0       0| = [x/(a*t), y/t, z*f/(f-n)+f*n/(n-f), z] => [x*z/(a*t), y*z/t,f*n/(n-f)/z+f/(f-n),1]
               | 0       0    f/(f-n) 1|
               | 0       0    fn(n-f) 0|

// Ortho


*/


// row major
template<typename T>
class TMatrix4 {
	using TVector = TVector4<T>;
public:
	// ctor
	TMatrix4() {}
	TMatrix4(TVector _x, TVector _y, TVector _z, TVector _w) : x(_x), y(_y), z(_z), w(_w) { }
	TMatrix4(const TMatrix4& m) { *this = m; }
	TMatrix4(T m00, T m01, T m02, T m03, T m10, T m11, T m12, T m13, 
		T m20, T m21, T m22, T m23, T m30, T m31, T m32, T m33) : 
		TMatrix4(
			TVector(m00, m01, m02, m03), TVector(m10, m11, m12, m13),
			TVector(m20, m21, m22, m23), TVector(m30, m31, m32, m33)
		){}

	// operator
	TVector operator* (TVector v) const {
		// vector * matrix
		T fX = (m[0][0] * v[0]) + (m[1][0] * v[1]) + (m[2][0] * v[2]) + (m[3][0] * v[3]);
		T fY = (m[0][1] * v[0]) + (m[1][1] * v[1]) + (m[2][1] * v[2]) + (m[3][1] * v[3]);
		T fZ = (m[0][2] * v[0]) + (m[1][2] * v[1]) + (m[2][2] * v[2]) + (m[3][2] * v[3]);
		T fW = (m[0][3] * v[0]) + (m[1][3] * v[1]) + (m[2][3] * v[2]) + (m[3][3] * v[3]);
		return TVector(fx, fy, fz, fw);
	}

	TMatrix4 operator* (TMatrix4 m2) {
		// m * m2
		TMatrix4 r;
		// Cache the invariants in registers
		T a0 = m[0][0], a1 = m[1][0], a2 = m[2][0], a3 = m[3][0];
		r[0][0] = (m2[0][0] * a0) + (m2[0][1] * a1) + (m2[0][2] * a2) + (m2[0][3] * a3);
		r[1][0] = (m2[1][0] * a0) + (m2[1][1] * a1) + (m2[1][2] * a2) + (m2[1][3] * a3);
		r[2][0] = (m2[2][0] * a0) + (m2[2][1] * a1) + (m2[2][2] * a2) + (m2[2][3] * a3);
		r[3][0] = (m2[3][0] * a0) + (m2[3][1] * a1) + (m2[3][2] * a2) + (m2[3][3] * a3);
		
		a0 = m[0][1], a1 = m[1][1], a2 = m[2][1], a3 = m[3][1];
		r[0][1] = (m2[0][0] * a0) + (m2[0][1] * a1) + (m2[0][2] * a2) + (m2[0][3] * a3);
		r[1][1] = (m2[1][0] * a0) + (m2[1][1] * a1) + (m2[1][2] * a2) + (m2[1][3] * a3);
		r[2][1] = (m2[2][0] * a0) + (m2[2][1] * a1) + (m2[2][2] * a2) + (m2[2][3] * a3);
		r[3][1] = (m2[3][0] * a0) + (m2[3][1] * a1) + (m2[3][2] * a2) + (m2[3][3] * a3);
		
		a0 = m[0][2], a1 = m[1][2], a2 = m[2][2], a3 = m[3][2];
		r[0][2] = (m2[0][0] * a0) + (m2[0][1] * a1) + (m2[0][2] * a2) + (m2[0][3] * a3);
		r[1][2] = (m2[1][0] * a0) + (m2[1][1] * a1) + (m2[1][2] * a2) + (m2[1][3] * a3);
		r[2][2] = (m2[2][0] * a0) + (m2[2][1] * a1) + (m2[2][2] * a2) + (m2[2][3] * a3);
		r[3][2] = (m2[3][0] * a0) + (m2[3][1] * a1) + (m2[3][2] * a2) + (m2[3][3] * a3);
		
		a0 = m[0][3], a1 = m[1][3], a2 = m[2][3], a3 = m[3][3];
		r[0][3] = (m2[0][0] * a0) + (m2[0][1] * a1) + (m2[0][2] * a2) + (m2[0][3] * a3);
		r[1][3] = (m2[1][0] * a0) + (m2[1][1] * a1) + (m2[1][2] * a2) + (m2[1][3] * a3);
		r[2][3] = (m2[2][0] * a0) + (m2[2][1] * a1) + (m2[2][2] * a2) + (m2[2][3] * a3);
		r[3][3] = (m2[3][0] * a0) + (m2[3][1] * a1) + (m2[3][2] * a2) + (m2[3][3] * a3);
		return r;
	}


	TVector& operator [](int idx) { return m[idx]; }
	const TVector operator [](int idx) const { return m[idx]; }

	// data
	union {
		struct { TVector x, y, z, w; };
		TVector m[4];
	};

	const static TMatrix4 Zero;
	const static TMatrix4 Identity;
};


template<typename T> const TMatrix4<T> TMatrix4<T>::Zero(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
template<typename T> const TMatrix4<T> TMatrix4<T>::Identity(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);


typedef TMatrix4<float> Matrix4;
typedef TMatrix4<float> Matrix4F;


}