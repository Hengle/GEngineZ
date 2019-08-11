#pragma once
#include <Core/CoreHeader.h>

namespace z {

enum EInput {
	EI_KEY_NONE = 0x0000,
	// act
	EI_ACT_DOWN  = 0x0001,
	EI_ACT_UP    = 0x0002,
	EI_ACT_MOVE  = 0x0004,
	EI_ACT_ROLL  = 0x0008,
	// modify 
	EI_BTN_CTRL  = 0x0001,
	EI_BTN_SHIFT = 0x0002,
	EI_BTN_ALT   = 0x0004,
	// mouse
	EI_BTN_ML   = 0x0010,
	EI_BTN_MM   = 0x0020,
	EI_BTN_MR   = 0x0040,
// ---- must by less 0xFF(uint8_t) ----
	
	// button
	EI_BTN_A = 0x0100,
	EI_BTN_B,
	EI_BTN_C,
	EI_BTN_D,
	EI_BTN_E,
	EI_BTN_F,
	EI_BTN_G,
	EI_BTN_H,
	EI_BTN_I,
	EI_BTN_J,
	EI_BTN_K,
	EI_BTN_L,
	EI_BTN_M,
	EI_BTN_N,
	EI_BTN_O,
	EI_BTN_P,
	EI_BTN_Q,
	EI_BTN_R,
	EI_BTN_S,
	EI_BTN_T,
	EI_BTN_U,
	EI_BTN_V,
	EI_BTN_W,
	EI_BTN_X,
	EI_BTN_Y,
	EI_BTN_Z,
	EI_BTN_ARROW_UP,
	EI_BTN_ARROW_DOWN,
	EI_BTN_ARROW_LEFT,
	EI_BTN_ARROW_RIGHT,
	EI_BTN_ENTER,
	EI_BTN_END,

	EI_MODIFY_BTN = EI_BTN_CTRL | EI_BTN_ALT | EI_BTN_SHIFT,
	EI_MOUSE_BTN = EI_BTN_ML | EI_BTN_MR | EI_BTN_MM,
	EI_BTN_BEGIN = EI_BTN_A,
	EI_MAX = EI_BTN_END
};

typedef std::function<void(EInput key, float, float)> InputMoveCallback;
typedef std::function<void(EInput key, float)> InputRollCallback;
typedef std::function<void(EInput key, EInput act)> InputClickCallback;

struct InputEvent {
	static int KeyEventIdAdder;
	InputEvent(EInput key, uint8_t act, uint8_t modify) :
		ID (++KeyEventIdAdder),
		Modify(modify),
		Act(act),
		Key(key) {
		//Callback.MoveCB = nullptr;
	}
	~InputEvent() {}

	uint8_t Modify;
	uint8_t Act;
	EInput Key;
	int ID;
	//union {
		InputMoveCallback MoveCB;
		InputClickCallback ClickCB;
		InputRollCallback RollCB;
	//};
};

class Input {
public:
	Input();
	~Input();

	void Dispatch();

	void OnMouseMove(float x, float y);
	void OnMouseKey(EInput key, EInput act);
	void OnMouseWheel(float delta);
	void OnKeyboard(EInput key, EInput act);

	int RegisterEvent(EInput key, EInput act, uint8_t modify, const InputMoveCallback& cb);
	int RegisterEvent(EInput key, EInput act, uint8_t modify, const InputRollCallback& cb);
	int RegisterEvent(EInput key, EInput act, uint8_t modify, const InputClickCallback& cb);
	void UnRegisterEvent(int id);

	float GetLastX() { return mLastX; }
	float GetLastY() { return mLastY; }

private:
	// <key, act> -> event list
	void PushEvent(InputEvent*);
	std::map<std::pair<EInput, EInput>, std::vector<InputEvent*>> mEvents;

	// record event in this frame
	uint8_t mModifyKey;
	uint8_t mMouseKey;
	bool mIsMouseMoved;
	bool mIsMouseRolled;

	float mCurX, mCurY;
	float mLastX, mLastY;
	float mWheelTotal;
	std::unordered_set<EInput> mDownkeys;
	std::unordered_set<EInput> mUpkeys;
};

extern Input *GInput;


}