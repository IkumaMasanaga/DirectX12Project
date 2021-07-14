#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")
#include "../system/window.h"
#include "input.h"


namespace lib {

	static UINT key_codes[] = {
		DIK_BACKSPACE,	// KEY_BACK BackSpaceキー
		DIK_TAB,		// KEY_TAB Tabキー
		DIK_RETURN,		// KEY_RETURN Enterキー

		DIK_LSHIFT,		// KEY_LSHIFT 左Shiftキー
		DIK_RSHIFT,		// KEY_RSHIFT 右Shiftキー
		DIK_LCONTROL,	// KEY_LCONTROL 左Ctrlキー
		DIK_RCONTROL,	// KEY_RCONTROL 右Ctrlキー
		DIK_ESCAPE,		// KEY_ESCAPE Escキー
		DIK_SPACE,		// KEY_SPACE スペースキー
		DIK_PGUP,		// KEY_PGUP PageUpキー
		DIK_PGDN,		// KEY_PGDN PageDownキー
		DIK_END,		// KEY_END Endキー
		DIK_HOME,		// KEY_HOME Homeキー
		DIK_LEFT,		// KEY_LEFT 左キー
		DIK_UP,			// KEY_UP 上キー
		DIK_RIGHT,		// KEY_RIGHT 右キー
		DIK_DOWN,		// KEY_DOWN 下キー
		DIK_INSERT,		// KEY_INSERT Insertキー
		DIK_DELETE,		// KEY_DELETE Deleteキー

		DIK_MINUS,		// KEY_MINUS －キー
		DIK_YEN,		// KEY_YEN ￥キー
		DIK_PREVTRACK,	// KEY_PREVTRACK ＾キー
		DIK_PERIOD,		// KEY_PERIOD ．キー
		DIK_SLASH,		// KEY_SLASH ／キー
		DIK_LALT,		// KEY_LALT 左Altキー
		DIK_RALT,		// KEY_RALT 右Altキー
		DIK_SCROLL,		// KEY_SCROLL ScrollLockキー
		DIK_SEMICOLON,	// KEY_SEMICOLON ；キー
		DIK_COLON,		// KEY_COLON ：キー
		DIK_LBRACKET,	// KEY_LBRACKET ［キー
		DIK_RBRACKET,	// KEY_RBRACKET ］キー
		DIK_AT,			// KEY_AT ＠キー
		DIK_BACKSLASH,	// KEY_BACKSLASH ＼キー
		DIK_COMMA,		// KEY_COMMA ，キー
		DIK_KANJI,		// KEY_KANJI 漢字キー
		DIK_CONVERT,	// KEY_CONVERT 変換キー
		DIK_NOCONVERT,	// KEY_NOCONVERT 無変換キー
		DIK_KANA,		// KEY_KANA カナキー
		DIK_APPS,		// KEY_APPS アプリケーションメニューキー
		DIK_CAPSLOCK,	// KEY_CAPSLOCK CaspLockキー
		DIK_SYSRQ,		// KEY_SYSRQ PrintScreenキー
		DIK_PAUSE,		// KEY_PAUSE PauseBreakキー
		DIK_LWIN,		// KEY_LWIN 左Winキー
		DIK_RWIN,		// KEY_RWIN 右Winキー

		DIK_NUMLOCK,	// KEY_NUMLOCK テンキーNumLockキー
		DIK_NUMPAD0,	// KEY_NUMPAD0 テンキー０
		DIK_NUMPAD1,	// KEY_NUMPAD1 テンキー１
		DIK_NUMPAD2,	// KEY_NUMPAD2 テンキー２
		DIK_NUMPAD3,	// KEY_NUMPAD3 テンキー３
		DIK_NUMPAD4,	// KEY_NUMPAD4 テンキー４
		DIK_NUMPAD5,	// KEY_NUMPAD5 テンキー５
		DIK_NUMPAD6,	// KEY_NUMPAD6 テンキー６
		DIK_NUMPAD7,	// KEY_NUMPAD7 テンキー７
		DIK_NUMPAD8,	// KEY_NUMPAD8 テンキー８
		DIK_NUMPAD9,	// KEY_NUMPAD9 テンキー９
		DIK_MULTIPLY,	// KEY_MULTIPLY テンキー＊キー
		DIK_ADD,		// KEY_ADD テンキー＋キー
		DIK_SUBTRACT,	// KEY_SUBTRACT テンキー－キー
		DIK_DECIMAL,	// KEY_DECIMAL テンキー．キー
		DIK_DIVIDE,		// KEY_DIVIDE テンキー／キー
		DIK_NUMPADENTER,// KEY_NUMPADENTER テンキーのエンターキー

		DIK_F1,			// KEY_F1 Ｆ１キー
		DIK_F2,			// KEY_F2 Ｆ２キー
		DIK_F3,			// KEY_F3 Ｆ３キー
		DIK_F4,			// KEY_F4 Ｆ４キー
		DIK_F5,			// KEY_F5 Ｆ５キー
		DIK_F6,			// KEY_F6 Ｆ６キー
		DIK_F7,			// KEY_F7 Ｆ７キー
		DIK_F8,			// KEY_F8 Ｆ８キー
		DIK_F9,			// KEY_F9 Ｆ９キー
		DIK_F10,		// KEY_F10 Ｆ１０キー
		DIK_F11,		// KEY_F11 Ｆ１１キー
		DIK_F12,		// KEY_F12 Ｆ１２キー

		DIK_A,			// KEY_A Ａキー
		DIK_B,			// KEY_B Ｂキー
		DIK_C,			// KEY_C Ｃキー
		DIK_D,			// KEY_D Ｄキー
		DIK_E,			// KEY_E Ｅキー
		DIK_F,			// KEY_F Ｆキー
		DIK_G,			// KEY_G Ｇキー
		DIK_H,			// KEY_H Ｈキー
		DIK_I,			// KEY_I Ｉキー
		DIK_J,			// KEY_J Ｊキー
		DIK_K,			// KEY_K Ｋキー
		DIK_L,			// KEY_L Ｌキー
		DIK_M,			// KEY_M Ｍキー
		DIK_N,			// KEY_N Ｎキー
		DIK_O,			// KEY_O Ｏキー
		DIK_P,			// KEY_P Ｐキー
		DIK_Q,			// KEY_Q Ｑキー
		DIK_R,			// KEY_R Ｒキー
		DIK_S,			// KEY_S Ｓキー
		DIK_T,			// KEY_T Ｔキー
		DIK_U,			// KEY_U Ｕキー
		DIK_V,			// KEY_V Ｖキー
		DIK_W,			// KEY_W Ｗキー
		DIK_X,			// KEY_X Ｘキー
		DIK_Y,			// KEY_Y Ｙキー
		DIK_Z,			// KEY_Z Ｚキー

		DIK_0,			// KEY_0 ０キー
		DIK_1,			// KEY_1 １キー
		DIK_2,			// KEY_2 ２キー
		DIK_3,			// KEY_3 ３キー
		DIK_4,			// KEY_4 ４キー
		DIK_5,			// KEY_5 ５キー
		DIK_6,			// KEY_6 ６キー
		DIK_7,			// KEY_7 ７キー
		DIK_8,			// KEY_8 ８キー
		DIK_9,			// KEY_9 ９キー
	};

	bool Input::initialize() {

		memset(key_now_, 0, sizeof(key_now_));
		memset(key_prev_, 0, sizeof(key_prev_));

		// インターフェースの作成
		if (FAILED(DirectInput8Create(GetModuleHandle(0), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)(&input_), NULL))) return false;

		// デバイスの作成
		if (FAILED(input_->CreateDevice(GUID_SysKeyboard, &device_, NULL))) return false;

		// フォーマットの設定
		// キー             ... c_dfDIKeyboard
		// マウス           ... c_dfDIMouse
		// ジョイスティック ... c_dfDIJoystick
		if (FAILED(device_->SetDataFormat(&c_dfDIKeyboard))) return false;

		// 協調モードの設定
		// フォアグラウンド or バックグラウンド
		// 排他的 or 非排他的
		if (FAILED(device_->SetCooperativeLevel(sys::Window::handle_, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND))) return false;

		// デバイス制御開始
		if (FAILED(device_->Acquire())) return false;

		return true;
	}

	bool Input::update() {
		memcpy(key_prev_, key_now_, sizeof(key_prev_));
		if (FAILED(device_->GetDeviceState(KEY_MAX, key_now_))) return false;
		return true;
	}

	bool Input::finalize() {

		// デバイス制御の停止
		if (FAILED(device_->Unacquire())) return false;

		device_->Release();
		input_->Release();
		
		return true;
	}

	bool Input::isKeyDown(const KeyCode key_code) {
		return (key_now_[key_codes[(int)key_code]] & 0x80);
	}

	bool Input::isKeyDownTrigger(const KeyCode key_code) {
		return (key_now_[key_codes[(int)key_code]] & 0x80) && !(key_prev_[key_codes[(int)key_code]] & 0x80);
	}

	bool Input::isKeyReleaseTrigger(const KeyCode key_code) {
		return !(key_now_[key_codes[(int)key_code]] & 0x80) && (key_prev_[key_codes[(int)key_code]] & 0x80);
	}

}
