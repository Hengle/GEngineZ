#include "Camera.h"

namespace z {


Camera::Camera(const math::Vector3F& eye, const math::Vector3F& target) :
	mMathCam(eye, target),
	mFov(math::K_PI / 3), 
	mAspect(1.0f),
	mNear(0.1),
	mFar(3000) {
	mMathCam.SetPerspective(mFov, mAspect, mNear, mFar);
}


}