#define PI 3.141592654

// copy from ue begin ======================== 
float Square(float x) {
	return x * x;
}

float2 Square(float2 x) {
	return x * x;
}

float3 Square(float3 x) {
	return x * x;
}

float Pow5(float x) {
	float xx = x * x;
	return xx * xx * x;
}

float ClampedPow(float X, float Y) {
	return pow(max(abs(X), 0.000001f), Y);
}

float PhongShadingPow(float X, float Y) {
	// The following clamping is done to prevent NaN being the result of the specular power computation.
	// Clamping has a minor performance cost.

	// In HLSL pow(a, b) is implemented as exp2(log2(a) * b).

	// For a=0 this becomes exp2(-inf * 0) = exp2(NaN) = NaN.

	// As seen in #TTP 160394 "QA Regression: PS3: Some maps have black pixelated artifacting."
	// this can cause severe image artifacts (problem was caused by specular power of 0, lightshafts propagated this to other pixels).
	// The problem appeared on PlayStation 3 but can also happen on similar PC NVidia hardware.

	// In order to avoid platform differences and rarely occuring image atrifacts we clamp the base.

	// Note: Clamping the exponent seemed to fix the issue mentioned TTP but we decided to fix the root and accept the
	// minor performance cost.

	return ClampedPow(X, Y);
}

// copy from ue end ======================== 