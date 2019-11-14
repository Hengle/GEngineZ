#pragma once

#include <Client/Scene/Camera.h>
#include <Client/Main/Input.h>
#include <Client/Main/Director.h>

namespace z {

class CameraController : public RefCounter {
public:
	CameraController() : 
		mCam{nullptr}, 
		mSensityRotate(1.5), 
		mSensityMove(1.0),
		mCtrlMouseKey(EI_BTN_MR) {

		using namespace std::placeholders;
		InputClickCallback btnActclickCb = std::bind(&CameraController::OnDirectionKey, this, _1, _2);
		GInput->RegisterEvent(EI_BTN_A, EI_ACT_DOWN, EI_KEY_NONE, btnActclickCb);
		GInput->RegisterEvent(EI_BTN_A, EI_ACT_UP, EI_KEY_NONE, btnActclickCb);
		GInput->RegisterEvent(EI_BTN_S, EI_ACT_DOWN, EI_KEY_NONE, btnActclickCb);
		GInput->RegisterEvent(EI_BTN_S, EI_ACT_UP, EI_KEY_NONE, btnActclickCb);
		GInput->RegisterEvent(EI_BTN_D, EI_ACT_DOWN, EI_KEY_NONE, btnActclickCb);
		GInput->RegisterEvent(EI_BTN_D, EI_ACT_UP, EI_KEY_NONE, btnActclickCb);
		GInput->RegisterEvent(EI_BTN_W, EI_ACT_DOWN, EI_KEY_NONE, btnActclickCb);
		GInput->RegisterEvent(EI_BTN_W, EI_ACT_UP, EI_KEY_NONE, btnActclickCb);

	
		InputClickCallback wantSelectCb = std::bind(&CameraController::OnWantSelect, this, _1, _2);
		GInput->RegisterEvent(EI_BTN_ML, EI_ACT_DOWN, EI_BTN_CTRL, wantSelectCb);

		InputMoveCallback mouseMoveCb = std::bind(&CameraController::OnMouseMove, this, _1, _2, _3);
		GInput->RegisterEvent(mCtrlMouseKey, EI_ACT_MOVE, EI_KEY_NONE, mouseMoveCb);
		GInput->RegisterEvent(EI_BTN_MM, EI_ACT_MOVE, EI_KEY_NONE, mouseMoveCb);

		InputRollCallback mouseRollCb = std::bind(&CameraController::OnMouseRoll, this, _1, _2);
		GInput->RegisterEvent(EI_BTN_MM, EI_ACT_ROLL, EI_KEY_NONE, mouseRollCb);
	}

	void SetCamera(Camera* cam) {
		mCam = cam;
	}

	void OnWantSelect(EInput key, uint8_t act) {
		z::Log<LINFO>() << GInput->GetLastX() << GInput->GetLastY();;
		
	}

	void OnDirectionKey(EInput key, uint8_t act) {
		if (mCam == nullptr) {
			return;
		}
		if (act == EI_ACT_DOWN) {
			float xdir = (key == EI_BTN_D ? 1.f : key == EI_BTN_A ? -1.f : 0.f);
			float ydir = (key == EI_BTN_W ? 1.f : key == EI_BTN_S ? -1.f : 0.f);
			if (xdir) mKeyDirPressed.x = xdir;
			if (ydir) mKeyDirPressed.y = ydir;
		} else if (act == EI_ACT_UP) {
			if (key == EI_BTN_D || key == EI_BTN_A) mKeyDirPressed.x = 0.f;
			if (key == EI_BTN_W || key == EI_BTN_S) mKeyDirPressed.y = 0.f;
		}

	}

	void OnMouseMove(EInput key, float x, float y) {
		if (mCam == nullptr) {
			return;
		}
		if (key == mCtrlMouseKey) {
			float deltaX = (x - GInput->GetLastX()) * mSensityRotate * 0.02;
			float deltaY = (y - GInput->GetLastY()) * mSensityRotate * 0.03;

			math::Camera* cam = mCam->GetCam();
			math::Matrix4F m1 = math::MatrixRotationAxis(cam->GetRight(), deltaY * math::ToRadian(1.0f));
			math::Matrix4F m2 = math::MatrixRotationAxis(cam->GetWorldUp(), deltaX * math::ToRadian(1.0f));
			math::Vector4F newF = m1 * (m2 * math::Vector4F(cam->GetForward(), 0.f));
			cam->UpdateForward(newF);

		} else if (key == EI_BTN_MM) {
			float deltaX = (x - GInput->GetLastX()) * mSensityMove * 0.01;
			float deltaY = (GInput->GetLastY() - y) * mSensityMove * 0.01;

			math::Camera* cam = mCam->GetCam();
			math::Vector3F newP = cam->GetPosition() + cam->GetWorldUp() * deltaY + cam->GetRight() * deltaX;
			cam->UpdatePosition(newP);
		}
	}

	void OnMouseRoll(EInput key, float delta) {
		delta = delta * GDirector->GetFrameTime() * 1.5f * mSensityMove;
		math::Camera* cam = mCam->GetCam();
		math::Vector3F newP = cam->GetPosition() + cam->GetForward() * delta;
		cam->UpdatePosition(newP);
	}

	void Apply() {
		// more smoothly
		if (mCam) {
			math::Camera* cam = mCam->GetCam();
			math::Vector3F delta = math::Vector3F(mKeyDirPressed.x, mKeyDirPressed.y, 0.f) * mSensityMove * GDirector->GetFrameTime() * 5.f;
			math::Vector3F newP = cam->GetPosition() + cam->GetForward() * delta.y + cam->GetRight() * delta.x;
			cam->UpdatePosition(newP);
		}
	}

	float GetSensityMove() {
		return mSensityMove;
	}

	float GetSensityRotate() {
		return mSensityRotate;
	}

	void SetSensityMove(float v) {
		mSensityMove = v;
	}

	void SetSensityRotate(float v) {
		mSensityRotate = v;
	}

private:
	math::Vector3F mKeyDirPressed;

	EInput mCtrlMouseKey;

	float mSensityMove;
	float mSensityRotate;

	Camera* mCam;
	

};

}