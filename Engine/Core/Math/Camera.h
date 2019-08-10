#pragma once

#include "Matrix.h"
#include "Function.h"

namespace z { namespace math {

class Camera {
public:
	Camera(const Vector3F& eye, const Vector3F& target, const Vector3F& worldUp = Vector3F(0, 1, 0)) {
		mPostion = eye;
		Vector3 forward = Normalize(target - eye);
		Vector3 right = Normalize(Cross(worldUp, forward));
		Vector3 up = Normalize(Cross(forward, right));

		mViewMatrix = Matrix4(
			{ right,   -Dot(right, eye) },
			{ up,      -Dot(up, eye) },
			{ forward, -Dot(forward, eye) },
			{ 0, 0, 0, 1 }
		);
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

	
	Vector3F GetRight() {
		return mViewMatrix[0];
	}

	Vector3F GetForward() {
		return mViewMatrix[1];
	}

	Vector3F GetUp() {
		return mViewMatrix[2];
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
	Vector3F mPostion;
};

}
}