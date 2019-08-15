#pragma once
#include <Core/CoreHeader.h>

namespace z {

/*
scaleX * R00 scaleX * R10 scaleX * R20 0
scaleY * R01 sacleY * R11 scaleY * R21 0
scaleZ * R02 scaleZ * R12 scaleZ * R22 0
Px           Py           pz           1

Rotator (Pitch(X), Yaw(Y), Roll(Z))
Rotate Order : YXZ (Yaw->Pitch->Roll)

*/

class Transform {
public:
	Transform() : 
		mPosition(math::Vector3F::Zero), 
		mScale(math::Vector3F::Identity),
		mRotation(math::Matrix4F::Identity) {
	}

	void SetTransform(const math::Matrix4F& t) {
		mPosition = { t[0][3], t[1][3], t[2][3] };
		mScale = {
			math::GetLength(math::Vector3F(t[0][0], t[1][0], t[2][0])),
			math::GetLength(math::Vector3F(t[0][0], t[1][0], t[2][0])),
			math::GetLength(math::Vector3F(t[0][0], t[1][0], t[2][0])),
		};
		mRotation = {
			{math::Vector3F{t[0][0], t[0][1], t[0][2]} / mScale, 0},
			{math::Vector3F{t[1][0], t[1][1], t[1][2]} / mScale, 0},
			{math::Vector3F{t[2][0], t[2][1], t[2][2]} / mScale, 0},
			{0, 0, 0, 1}
		};

	}

	math::Matrix4F GetTransform() {
		return math::Matrix4F{
			{ math::Vector3F(mRotation[0]) * mScale, mPosition.x },
			{ math::Vector3F(mRotation[1]) * mScale, mPosition.y },
			{ math::Vector3F(mRotation[2]) * mScale, mPosition.z },
			{ 0, 0, 0, 1}
		};
	}

	void SetPostion(const math::Vector3F& pos) {
		mPosition = pos;
	}

	void SetScale(const math::Vector3F& scale) {
		mScale = scale;
	}

	void SetRotator(math::Vector3F rotator) {
		mRotation = math::Matrix4F::Identity;
		std::cout << mRotation << std::endl;
		std::cout << math::MatrixRotationY(rotator.y) << std::endl;
		std::cout << math::MatrixRotationZ(rotator.z) << std::endl;
		std::cout << math::MatrixRotationX(rotator.x) << std::endl;

		mRotation = mRotation * math::MatrixRotationY(rotator.y) * math::MatrixRotationX(rotator.x) * math::MatrixRotationZ(rotator.z);

		std::cout << mRotation << std::endl;
	}

	math::Vector3F GetPosition() {
		return mPosition;
	}

	math::Vector3F GetRotator() {
		math::Vector3F euler;
		euler.x = std::asinf(mRotation[2][1]);
		if (std::cosf(euler.x) > 0.0001) {
			euler.y = -std::atan2f(mRotation[2][0], mRotation[2][2]);
			euler.z = -std::atan2f(mRotation[0][1], mRotation[1][1]);
		} else {
			euler.y = 0.0f;
			euler.z = std::atan2f(mRotation[0][1], mRotation[0][0]);
		}
		return euler;
	}

	math::Vector3F GetScale() {
		return mScale;
	}

private:
	math::Vector3F mPosition;
	math::Vector3F mScale;
	math::Matrix4F mRotation;
};

}