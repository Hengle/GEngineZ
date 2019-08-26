#pragma once
#include <Core/CoreHeader.h>

namespace z {
class CameraController;

class Camera : public RefCounter {
public:
	friend class CameraController;
	Camera(const math::Vector3F& eye, const math::Vector3F& target);


	void SetAspect(float aspect) {
		mAspect = aspect;
		mMathCam.SetPerspective(mFov, mAspect, mNear, mFar);
	}

	void SetFov(float fov) {
		mFov = fov;
		mMathCam.SetPerspective(mFov, mAspect, mNear, mFar);
	}

 
	math::Camera *GetCam() {
		return &mMathCam;
	}

private:
	math::Camera mMathCam;

	float mFov;
	float mAspect;
	float mNear;
	float mFar;
};


}