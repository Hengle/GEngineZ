#pragma once

namespace z {
namespace math {

const float K_PI      = 3.141592654f;
const float K_2PI     = 6.283185307f;
const float K_1DIVPI  = 0.318309886f;
const float K_1DIV2PI = 0.159154943f;
const float K_PIDIV2  = 1.570796327f;
const float K_PIDIV4  = 0.785398163f;

// degree
inline float ToRadian(float degree) {
	return degree / 180.0f * K_PI;
}

inline float ToDegree(float radian) {
	return radian * 180.0f / K_PI;
}

// eps
const float eps = (float)1e-6;

inline float EpsEqual(float a, float b) {
	return a - b > -eps && a - b < eps;
}

}
}