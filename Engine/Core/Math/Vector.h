#pragma once

namespace z {

struct Zero {};
struct Identity {};

template <typename T>
class TVector4 {
public:
	TVector4() {}
	TVector4(T v) : TVector4(v, v, v, v) {}
	TVector4(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}
	TVector4(const TVector4& v) { *this = v; }
	TVector4(Zero) : TVector4(0, 0, 0, 0){}
	TVector4(Identity) : TVector4(1, 1, 1, 1) {}


	TVector4 operator- () const { return TVector4(-x, -y, -z. - w); }
	TVector4 operator+ (TVector4 v2) const { return Vector4(x + v2.x, y + v2.y, z + v2.z, w + v2.w); }
	TVector4 operator- (TVector4 v2) const { return Vector4(x - v2.x, y - v2.y, z - v2.z, w - v2.w); }
	TVector4 operator* (TVector4 v2) const { return Vector4(x * v2.x, y * v2.y, z * v2.z, w * v2.w); }
	TVector4 operator/ (TVector4 v2) const { return Vector4(x / v2.x, y / v2.y, z / v2.z, w / v2.w); }
	TVector4 operator* (T v2) const { return *this * Vector4(v2); }
	TVector4 operator/ (T v2) const { return *this / Vector4(v2); }

	TVector4& operator += (TVector4 v2) { *this = *this + v2; return *this; }
	TVector4& operator -= (TVector4 v2) { *this = *this - v2; return *this; }
	TVector4& operator *= (TVector4 v2) { *this = *this * v2; return *this; }
	TVector4& operator /= (TVector4 v2) { *this = *this / v2; return *this; }

	friend TVector4 operator* (T v1, TVector4 v2) { return TVector4(v1) * v2; }
	friend TVector4 operator/ (T v1, TVector4 v2) { return TVector4(v1) / v2; }

	T operator [](int idx) const { return t[idx]; }
	union {
		struct { T x, y, z, w; };
		T t[4];
	};

};

typedef TVector4<float> Vector4;

}