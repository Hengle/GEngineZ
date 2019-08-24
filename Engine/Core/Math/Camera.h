#pragma once

#include "Matrix.h"
#include "LinearAlg.h"

namespace z { namespace math {


inline Matrix4F MatrixPerspective(float fov, float aspect, float nearZ, float farZ) {
	float tanv = std::sin(fov / 2) / std::cos(fov / 2);
	float r = farZ / (farZ - nearZ);

	return Matrix4F(
		1 / tanv / aspect, 0, 0, 0,
		0, 1 / tanv, 0, 0,
		0, 0, r, -r * nearZ,
		0, 0, 1, 0
	);
}

inline Matrix4F MatrixOrtho(float left, float right, float bottom, float top, float nearZ, float farZ) {
	return Matrix4F(
		{ 2.0f / (right - left), 0.0f, 0.0f, 0.0f },
		{ 0.0f, 2.0f / (top - bottom), 0.0f, 0.0f },
		{ 0.0f, 0.0f, 2.0f / (farZ - nearZ), 0.0f },
		{ (left+right)/(left-right),  (top+bottom)/(bottom-top), (nearZ+farZ)/(nearZ-farZ), 1.0f }
	);
}


inline Matrix4F MatrixOrtho2D(float left, float right, float bottom, float top) {
	return Matrix4F(
		{ 2.0f / (right - left), 0.0f, 0.0f, 0.0f },
		{ 0.0f, 2.0f / (top - bottom), 0.0f, 0.0f },
		{ 0.0f, 0.0f, 1.0f, 0.0f },
		{ (left + right) / (left - right),  (top + bottom) / (bottom - top), 0.0f, 1.0f }
	);
}

class Camera {
public:
	Camera(const Vector3F& eye, const Vector3F& target, const Vector3F& worldUp = Vector3F(0, 1, 0)) {
		mPosition = eye;
		mWorldUp = worldUp;
		UpdateForward(Normalize(target - eye));
	}

	void SetPerspective(float fov, float aspect, float nearZ, float farZ) {
		float tanv = std::sin(fov / 2) / std::cos(fov / 2);
		float r = farZ / (farZ - nearZ);

		mProjMatrix  = Matrix4(
			1/tanv/aspect, 0,      0, 0,
			0,             1/tanv, 0, 0,
			0,             0,      r, -r*nearZ,
			0,             0,      1, 0
		);
	}

	void UpdatePosition(const Vector3F& pos) {
		mPosition = pos;
		mViewMatrix[0][3] = -Dot(GetRight(), pos);
		mViewMatrix[1][3] = -Dot(GetUp(), pos);
		mViewMatrix[2][3] = -Dot(GetForward(), pos);
	}

	void UpdateForward(const Vector3F &forward) {
		Vector3 right = Normalize(Cross(mWorldUp, forward));
		Vector3 up = Normalize(Cross(forward, right));

		mViewMatrix = Matrix4(
			{ right,   -Dot(right, mPosition) },
			{ up,      -Dot(up, mPosition) },
			{ forward, -Dot(forward, mPosition) },
			{ 0, 0, 0, 1 }
		);
	}

	Vector3F GetPosition() {
		return mPosition;
	}
	
	Vector3F GetRight() {
		return mViewMatrix[0];
	}

	Vector3F GetForward() {
		return mViewMatrix[2];
	}

	Vector3F GetUp() {
		return mViewMatrix[1];
	}

	Vector3F GetWorldUp() {
		return mWorldUp;
	}

	Matrix4F GetViewMatrix() {
		return mViewMatrix;
	}

	Matrix4F GetViewProjectMatrix() {
		return mViewMatrix * mProjMatrix;
	}


private:
	Matrix4F mViewMatrix;
	Matrix4F mProjMatrix;
	Vector3F mPosition;
	Vector3F mWorldUp;
};

}
}