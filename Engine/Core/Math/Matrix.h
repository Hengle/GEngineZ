 #pragma once

#include "Vector.h"

namespace z {
namespace math {

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
l, r, b, t, f, n (left, right, bottom, top, far, near)
(l, r) -> (-1, 1), (b, t) -> (-1, 1), (n, f) -> (-1, 1)
                | 2/(r-l)       0        0  -(r+l)/(r-l) |
[x, y, z, 1] *  | 0       2/(t-b)        0  -(t+b)/(t-b) |
                | 0             0  2/(f-n)  -(f+n)/(f-n) |
                | 0             0        0             1 |

*/

// row major
// Matrix3
template<typename T>
class TMatrix3 {
	using TVector = TVector3<T>;
public:
	// ctor
	TMatrix3() {}
	TMatrix3(TVector _x, TVector _y, TVector _z) : x(_x), y(_y), z(_z) { }
	TMatrix3(const TMatrix3& m) { *this = m; }
	TMatrix3(T m00, T m01, T m02, T m10, T m11, T m12, T m20, T m21, T m22) :
		TMatrix3(TVector(m00, m01, m02), TVector(m10, m11, m12), TVector(m20, m21, m22)) {}

	// operator
	TVector operator* (const TVector& v) const {
		// vector * matrix
		T fX = (m00 * v[0]) + (m01 * v[1]) + (m02 * v[2]);
		T fY = (m10 * v[0]) + (m11 * v[1]) + (m12 * v[2]);
		T fZ = (m20 * v[0]) + (m21 * v[1]) + (m22 * v[2]);
		return TVector(fX, fY, fZ);
	}

	TMatrix3 operator* (const TMatrix3& m2) {
		// m * m2
		TMatrix3 r;
		T a0 = m[0][0], a1 = m[1][0], a2 = m[2][0];
		r[0][0] = (m2[0][0] * a0) + (m2[0][1] * a1) + (m2[0][2] * a2);
		r[1][0] = (m2[1][0] * a0) + (m2[1][1] * a1) + (m2[1][2] * a2);
		r[2][0] = (m2[2][0] * a0) + (m2[2][1] * a1) + (m2[2][2] * a2);

		a0 = m[0][1], a1 = m[1][1], a2 = m[2][1];
		r[0][1] = (m2[0][0] * a0) + (m2[0][1] * a1) + (m2[0][2] * a2);
		r[1][1] = (m2[1][0] * a0) + (m2[1][1] * a1) + (m2[1][2] * a2);
		r[2][1] = (m2[2][0] * a0) + (m2[2][1] * a1) + (m2[2][2] * a2);

		a0 = m[0][2], a1 = m[1][2], a2 = m[2][2];
		r[0][2] = (m2[0][0] * a0) + (m2[0][1] * a1) + (m2[0][2] * a2);
		r[1][2] = (m2[1][0] * a0) + (m2[1][1] * a1) + (m2[1][2] * a2);
		r[2][2] = (m2[2][0] * a0) + (m2[2][1] * a1) + (m2[2][2] * a2);

		return r;
	}

	TMatrix3& operator *= (const TMatrix3 &v2) { *this = *this * v2; return *this; }

	TMatrix3 GetInverse() {
		TMatrix3 r;
		r.m00 = m11 * m22 - m21 * m12;
		r.m10 = m20 * m12 - m10 * m22;
		r.m20 = m10 * m21 - m20 * m11;
		r.m01 = m21 * m02 - m01 * m22;
		r.m11 = m00 * m22 - m20 * m02;
		r.m21 = m20 * m01 - m00 * m21;
		r.m02 = m01 * m12 - m11 * m02;
		r.m12 = m10 * m02 - m00 * m12;
		r.m22 = m00 * m11 - m10 * m01;

		// Determinant
		T det = m00 * r.m00 + m10 * r.m01 + m20 * r.m02;

		CHECK(!Equal(det, .0f));
		det = 1.0f / det;

		r.m00 *= det, r.m01 *= det, r.m02 *= det;
		r.m10 *= det, r.m11 *= det, r.m12 *= det;
		r.m20 *= det, r.m21 *= det, r.m22 *= det;
		return r;
	}

	TMatrix3 GetTranspose() {
		return TMatrix3<T>{
			m[0][0], m[1][0], m[2][0],
			m[0][1], m[1][1], m[2][1],
			m[0][2], m[1][2], m[2][2]
		};
	}
	TVector& operator [](int idx) { return m[idx]; }
	const TVector operator [](int idx) const { return m[idx]; }

	friend std::ostream& operator<<(std::ostream& out, const TMatrix3& v) {
		out << "(" << v.x << ", " << v.y << ", " << v.z << ")";
		return out;
	}

	// data
	union {
		struct { TVector x, y, z; };
		struct { T m00, m01, m02, m10, m11, m12, m20, m21, m22; };
		TVector m[3];
	};

	const static TMatrix3 Zero;
	const static TMatrix3 Identity;
};


template<typename T> const TMatrix3<T> TMatrix3<T>::Zero(0, 0, 0, 0, 0, 0, 0, 0, 0);
template<typename T> const TMatrix3<T> TMatrix3<T>::Identity(1, 0, 0, 0, 1, 0, 0, 0, 1);

typedef TMatrix3<float> Matrix3F;


// Matrix4
template<typename T>
class TMatrix4 {
	using TVector = TVector4<T>;
public:
	// ctor
	TMatrix4() : x(0), y(0), z(0), w(0) {}
	TMatrix4(TVector _x, TVector _y, TVector _z, TVector _w) : x(_x), y(_y), z(_z), w(_w) { }
	TMatrix4(const TMatrix4& m) { *this = m; }
	TMatrix4(T m00, T m01, T m02, T m03, T m10, T m11, T m12, T m13,
		T m20, T m21, T m22, T m23, T m30, T m31, T m32, T m33) :
		TMatrix4(
			TVector(m00, m01, m02, m03), TVector(m10, m11, m12, m13),
			TVector(m20, m21, m22, m23), TVector(m30, m31, m32, m33)
		) {}

	// operator
	TVector operator* (const TVector& v) const {
		// vector * matrix
		T fX = (m[0][0] * v[0]) + (m[0][1] * v[1]) + (m[0][2] * v[2]) + (m[0][3] * v[3]);
		T fY = (m[1][0] * v[0]) + (m[1][1] * v[1]) + (m[1][2] * v[2]) + (m[1][3] * v[3]);
		T fZ = (m[2][0] * v[0]) + (m[2][1] * v[1]) + (m[2][2] * v[2]) + (m[2][3] * v[3]);
		T fW = (m[3][0] * v[0]) + (m[3][1] * v[1]) + (m[3][2] * v[2]) + (m[3][3] * v[3]);
		return TVector(fX, fY, fZ, fW);
	}

	TMatrix4 operator* (const TMatrix4& m2) {
		// m * m2
		TMatrix4 r;
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

	TMatrix4& operator *= (const TMatrix4& v2) { *this = *this * v2; return *this; }

	TMatrix4 GetInverse() {
		TMatrix4 r;
		T tmp[12];

		tmp[0] = m22 * m33;
		tmp[1] = m32 * m23;
		tmp[2] = m12 * m33;
		tmp[3] = m32 * m13;
		tmp[4] = m12 * m23;
		tmp[5] = m22 * m13;
		tmp[6] = m02 * m33;
		tmp[7] = m32 * m03;
		tmp[8] = m02 * m23;
		tmp[9] = m22 * m03;
		tmp[10] = m02 * m13;
		tmp[11] = m12 * m03;

		r.m00 = tmp[0] * m11 + tmp[3] * m21 + tmp[4 ] * m31 - tmp[1] * m11 - tmp[2] * m21 - tmp[5 ] * m31;
		r.m01 = tmp[1] * m01 + tmp[6] * m21 + tmp[9 ] * m31 - tmp[0] * m01 - tmp[7] * m21 - tmp[8 ] * m31;
		r.m02 = tmp[2] * m01 + tmp[7] * m11 + tmp[10] * m31 - tmp[3] * m01 - tmp[6] * m11 - tmp[11] * m31;
		r.m03 = tmp[5] * m01 + tmp[8] * m11 + tmp[11] * m21 - tmp[4] * m01 - tmp[9] * m11 - tmp[10] * m21;
		r.m10 = tmp[1] * m10 + tmp[2] * m20 + tmp[5 ] * m30 - tmp[0] * m10 - tmp[3] * m20 - tmp[4 ] * m30;
		r.m11 = tmp[0] * m00 + tmp[7] * m20 + tmp[8 ] * m30 - tmp[1] * m00 - tmp[6] * m20 - tmp[9 ] * m30;
		r.m12 = tmp[3] * m00 + tmp[6] * m10 + tmp[11] * m30 - tmp[2] * m00 - tmp[7] * m10 - tmp[10] * m30;
		r.m13 = tmp[4] * m00 + tmp[9] * m10 + tmp[10] * m20 - tmp[5] * m00 - tmp[8] * m10 - tmp[11] * m20;

		tmp[0] = m20 * m31;
		tmp[1] = m30 * m21;
		tmp[2] = m10 * m31;
		tmp[3] = m30 * m11;
		tmp[4] = m10 * m21;
		tmp[5] = m20 * m11;
		tmp[6] = m00 * m31;
		tmp[7] = m30 * m01;
		tmp[8] = m00 * m21;
		tmp[9] = m20 * m01;
		tmp[10] = m00 * m11;
		tmp[11] = m10 * m01;

		r.m20 = tmp[0 ] * m13 + tmp[3 ] * m23 + tmp[4 ] * m33 - tmp[1 ] * m13 - tmp[2 ] * m23 - tmp[5 ] * m33;
		r.m21 = tmp[1 ] * m03 + tmp[6 ] * m23 + tmp[9 ] * m33 - tmp[0 ] * m03 - tmp[7 ] * m23 - tmp[8 ] * m33;
		r.m22 = tmp[2 ] * m03 + tmp[7 ] * m13 + tmp[10] * m33 - tmp[3 ] * m03 - tmp[6 ] * m13 - tmp[11] * m33;
		r.m23 = tmp[5 ] * m03 + tmp[8 ] * m13 + tmp[11] * m23 - tmp[4 ] * m03 - tmp[9 ] * m13 - tmp[10] * m23;
		r.m30 = tmp[2 ] * m22 + tmp[5 ] * m32 + tmp[1 ] * m12 - tmp[4 ] * m32 - tmp[0 ] * m12 - tmp[3 ] * m22;
		r.m31 = tmp[8 ] * m32 + tmp[0 ] * m02 + tmp[7 ] * m22 - tmp[6 ] * m22 - tmp[9 ] * m32 - tmp[1 ] * m02;
		r.m32 = tmp[6 ] * m12 + tmp[11] * m32 + tmp[3 ] * m02 - tmp[10] * m32 - tmp[2 ] * m02 - tmp[7 ] * m12;
		r.m33 = tmp[10] * m22 + tmp[4 ] * m02 + tmp[9 ] * m12 - tmp[8 ] * m12 - tmp[11] * m22 - tmp[5 ] * m02;

		// Determinant
		float det = (m00 * r.m00 + m10 * r.m01 + m20 * r.m02 + m30 * r.m03);
		
		CHECK(!Equal(det, .0f));
		det = 1.0f / det;

		r.m00 *= det, r.m01 *= det, r.m02 *= det, r.m03 *= det;
		r.m10 *= det, r.m11 *= det, r.m12 *= det, r.m13 *= det;
		r.m20 *= det, r.m21 *= det, r.m22 *= det, r.m23 *= det;
		r.m30 *= det, r.m31 *= det, r.m32 *= det, r.m33 *= det;
		return r;
	}

	TMatrix4 GetTranspose() {
		return TMatrix4{
			m[0][0], m[1][0], m[2][0], m[3][0],
			m[0][1], m[1][1], m[2][1], m[3][1],
			m[0][2], m[1][2], m[2][2], m[3][2],
			m[0][3], m[1][3], m[2][3], m[3][3]
		};
	}
	TVector& operator [](int idx) { return m[idx]; }
	const TVector operator [](int idx) const { return m[idx]; }

	friend std::ostream& operator<<(std::ostream& out, const TMatrix4& v) {
		out << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
		return out;
	}

	operator TMatrix3<T>() { return TMatrix3<T>{x, y, z}; }

	// data
	union {
		struct { TVector x, y, z, w; };
		struct { T m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33; };
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
}