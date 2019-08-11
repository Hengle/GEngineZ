#pragma once

#include "Matrix.h"
#include "LinearAlg.h"

namespace z { namespace math {

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