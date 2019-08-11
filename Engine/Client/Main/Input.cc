#include "Input.h"

namespace z {

int InputEvent::KeyEventIdAdder = 0;

Input* GInput = nullptr;

Input::Input() :
	mModifyKey(0),
	mMouseKey(0),
	mIsMouseMoved(false),
	mIsMouseRolled(false),
	mCurX(0.f), mCurY(0.f),
	mLastX(0.f), mLastY(0.f),
	mWheelTotal(0.f) {
	InitializeSingleton<Input>(GInput, this);
}

Input::~Input() {
	FinalizeSingleton<Input>(GInput, this);
}

void Input::OnMouseMove(float x, float y) {
	mIsMouseMoved = true;
	mCurX = x, mCurY = y;
}

void Input::OnMouseKey(EInput key, EInput act) {
	if (act == EI_ACT_DOWN) {
		mDownkeys.insert(key);
		if (key & EI_MOUSE_BTN) {
			mMouseKey |= key;
		}
	} else if (act == EI_ACT_UP) {
		if (key & EI_MOUSE_BTN) {
			mMouseKey &= ~key;
		}
		mUpkeys.insert(key);
	}
}

void Input::OnMouseWheel(float delta) {
	mIsMouseRolled = true;
	mWheelTotal += delta;
}

void Input::OnKeyboard(EInput key, EInput act) {
	if (act == EI_ACT_DOWN) {
		mDownkeys.insert(key);
		if (key <= EI_MODIFY_BTN && (key & EI_MODIFY_BTN)) {
			mModifyKey |= key;
		}
	} else if (act == EI_ACT_UP) {
		mUpkeys.insert(key);
		if (key <= EI_MODIFY_BTN && (key & EI_MODIFY_BTN)) {
			mModifyKey &= ~key;
		}
	}
}

int Input::RegisterEvent(EInput key, EInput act, uint8_t modify, const InputMoveCallback& cb) {
	InputEvent* e = new InputEvent(key, act, modify);
	e->MoveCB = cb;
	PushEvent(e);
	return e->ID;
}

int Input::RegisterEvent(EInput key, EInput act, uint8_t modify, const InputRollCallback& cb) {
	InputEvent* e = new InputEvent(key, act, modify);
	e->RollCB = cb;
	PushEvent(e);
	return e->ID;
}

int Input::RegisterEvent(EInput key, EInput act, uint8_t modify, const InputClickCallback& cb) {
	InputEvent* e = new InputEvent(key, act, modify);
	e->ClickCB = std::move(cb);
	PushEvent(e);
	return e->ID;
}

void Input::PushEvent(InputEvent* e) {
	auto mapKey = std::make_pair((EInput)e->Key, (EInput)e->Act);
	if (mEvents.find(mapKey) == mEvents.end()) {
		mEvents[mapKey] = { e };
	} else {
		mEvents[mapKey].push_back(e);
	}
}

void Input::UnRegisterEvent(int id) {
	for (auto evIter : mEvents) {
		for (auto vecIter = evIter.second.begin(); vecIter != evIter.second.end(); vecIter++) {
			if ((*vecIter)->ID == id) {
				auto* e = *vecIter;
				evIter.second.erase(vecIter);
				delete e;
			}
		}
	}
}

void Input::Dispatch() {
	// handle mouse move
	if (mIsMouseMoved) {
		// only zero or one mouse key supported
		if (mMouseKey == 0 || (mMouseKey & (mMouseKey - 1)) == 0) {
			auto iter = mEvents.find(std::make_pair((EInput)mMouseKey, EI_ACT_MOVE));
			if (iter != mEvents.end()) {
				for (auto* ev : iter->second) {
					if (ev->Modify == mModifyKey) {
						ev->MoveCB((EInput)mMouseKey, mCurX, mCurY);
					}
				}
			}
		}
		mIsMouseMoved = false;
		mLastX = mCurX, mLastY = mCurY;
	}

	if (mIsMouseRolled) {
		auto iter = mEvents.find(std::make_pair(EI_BTN_MM, EI_ACT_ROLL));
		if (iter != mEvents.end()) {
			for (auto* ev : iter->second) {
				if (ev->Modify == mModifyKey) {
					ev->RollCB((EInput)mMouseKey, mWheelTotal);
				}
			}
		}
		mIsMouseRolled = false;
		mWheelTotal = 0;
	}

	for (auto& downKey : mDownkeys) {
		auto iter = mEvents.find(std::make_pair(downKey, EI_ACT_DOWN));
		if (iter != mEvents.end()) {
			for (auto* ev : iter->second) {
				if (ev->Modify == mModifyKey) {
					ev->ClickCB(downKey, EI_ACT_DOWN);
				}
			}
		}
	}
	mDownkeys.clear();

	for (auto& upKey : mUpkeys) {
		auto iter = mEvents.find(std::make_pair(upKey, EI_ACT_UP));
		if (iter != mEvents.end()) {
			for (auto* ev : iter->second) {
				if (ev->Modify == mModifyKey) {
					ev->ClickCB(upKey, EI_ACT_UP);
				}
			}
		}
	}
	mUpkeys.clear();
}

}