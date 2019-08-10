#pragma once
#include <Core/CoreHeader.h>

namespace z {

class Camera : public RefCounter {
public:
	Camera(const math::Vector3F& eye, const math::Vector3F& target);


	void SetAspect(float aspect) {
		mAspect = aspect;
		mMathCam.SetPerspective(mFov, mAspect, 1, 1000);
	}

	void SetFov(float fov) {
		mFov = fov;
		mMathCam.SetPerspective(mFov, mAspect, 1, 1000);
	}

	math::Camera *GetCam() {
		return &mMathCam;
	}

private:
	math::Camera mMathCam;

	float mFov;
	float mAspect;
};


}