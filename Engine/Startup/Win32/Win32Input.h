#pragma once
#include "Win32Window.h"
#include <Client/Main/Input.h>

namespace z {

class Win32InputHandler {
public:
	Win32InputHandler() :
		mCurMouseKey(0) {

		mKeyMap = new EInput[256];
		memset(mKeyMap, 0, 256 * sizeof(EInput));
		// A-Z
		for (int i = 0x41; i <= 0x5A; i++) {
			mKeyMap[i] = EInput(EI_BTN_A + (i - 0x41));
		}
		// Arrow
		mKeyMap[VK_UP]      = EI_BTN_ARROW_UP;
		mKeyMap[VK_DOWN]    = EI_BTN_ARROW_DOWN;
		mKeyMap[VK_LEFT]    = EI_BTN_ARROW_LEFT;
		mKeyMap[VK_RIGHT]   = EI_BTN_ARROW_RIGHT;
		// Ctrl/Alt/Shift
		mKeyMap[VK_CONTROL] = EI_BTN_CTRL;
		mKeyMap[VK_MENU]    = EI_BTN_ALT;
		mKeyMap[VK_SHIFT]   = EI_BTN_SHIFT;
		// Enter/Back/Delete
		mKeyMap[VK_RETURN]  = EI_BTN_ENTER;
		// 1-9

		// F1-F12

		// Other
	}

	std::vector<EInput> OnMouseDown(UINT_PTR key) {
		UINT_PTR new_key = key & ~mCurMouseKey;
		mCurMouseKey |= key;

		std::vector<EInput> ret;
		if (new_key & MK_LBUTTON) {
			ret.push_back(EI_BTN_ML);
		}
		if (new_key & MK_RBUTTON) {
			ret.push_back(EI_BTN_MR);
		}
		if (new_key & MK_MBUTTON) {
			ret.push_back(EI_BTN_MM);
		}
		return ret;
	}

	std::vector<EInput>  OnMouseUp(UINT_PTR key) {
		UINT_PTR new_key = ~key & mCurMouseKey;
		mCurMouseKey = key;

		std::vector<EInput> ret;
		if (new_key & MK_LBUTTON) {
			ret.push_back(EI_BTN_ML);
		}
		if (new_key & MK_RBUTTON) {
			ret.push_back(EI_BTN_MR);
		}
		if (new_key & MK_MBUTTON) {
			ret.push_back(EI_BTN_MM);
		}
		return ret;
	}

	EInput EncodeKey(UINT_PTR key) {
		if (key >= 256) {
			return (EInput)0;
		}
		return mKeyMap[key];
	}

private:
	UINT_PTR mCurMouseKey;
	EInput *mKeyMap;
	
};

}
