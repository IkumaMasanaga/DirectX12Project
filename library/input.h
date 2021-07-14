#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>


namespace lib {

	// キー列挙定数
	enum class KeyCode {
		KEY_BACK,			// BackSpaceキー
		KEY_TAB,			// Tabキー
		KEY_RETURN,			// Enterキー

		KEY_LSHIFT,			// 左Shiftキー
		KEY_RSHIFT,			// 右Shiftキー
		KEY_LCONTROL,		// 左Ctrlキー
		KEY_RCONTROL,		// 右Ctrlキー
		KEY_ESCAPE,			// Escキー
		KEY_SPACE,			// スペースキー
		KEY_PGUP,			// PageUpキー
		KEY_PGDN,			// PageDownキー
		KEY_END,			// Endキー
		KEY_HOME,			// Homeキー
		KEY_LEFT,			// 左キー
		KEY_UP,				// 上キー
		KEY_RIGHT,			// 右キー
		KEY_DOWN,			// 下キー
		KEY_INSERT,			// Insertキー
		KEY_DELETE,			// Deleteキー

		KEY_MINUS,			// －キー
		KEY_YEN,			// ￥キー
		KEY_PREVTRACK,		// ＾キー
		KEY_PERIOD,			// ．キー
		KEY_SLASH,			// ／キー
		KEY_LALT,			// 左Altキー
		KEY_RALT,			// 右Altキー
		KEY_SCROLL,			// ScrollLockキー
		KEY_SEMICOLON,		// ；キー
		KEY_COLON,			// ：キー
		KEY_LBRACKET,		// ［キー
		KEY_RBRACKET,		// ］キー
		KEY_AT,				// ＠キー
		KEY_BACKSLASH,		// ＼キー
		KEY_COMMA,			// ，キー
		KEY_KANJI,			// 漢字キー
		KEY_CONVERT,		// 変換キー
		KEY_NOCONVERT,		// 無変換キー
		KEY_KANA,			// カナキー
		KEY_APPS,			// アプリケーションメニューキー
		KEY_CAPSLOCK,		// CaspLockキー
		KEY_SYSRQ,			// PrintScreenキー
		KEY_PAUSE,			// PauseBreakキー
		KEY_LWIN,			// 左Winキー
		KEY_RWIN,			// 右Winキー

		KEY_NUMLOCK,		// テンキーNumLockキー
		KEY_NUMPAD0,		// テンキー０
		KEY_NUMPAD1,		// テンキー１
		KEY_NUMPAD2,		// テンキー２
		KEY_NUMPAD3,		// テンキー３
		KEY_NUMPAD4,		// テンキー４
		KEY_NUMPAD5,		// テンキー５
		KEY_NUMPAD6,		// テンキー６
		KEY_NUMPAD7,		// テンキー７
		KEY_NUMPAD8,		// テンキー８
		KEY_NUMPAD9,		// テンキー９
		KEY_MULTIPLY,		// テンキー＊キー
		KEY_ADD,			// テンキー＋キー
		KEY_SUBTRACT,		// テンキー－キー
		KEY_DECIMAL,		// テンキー．キー
		KEY_DIVIDE,			// テンキー／キー
		KEY_NUMPADENTER,	// テンキーのエンターキー

		KEY_F1,				// Ｆ１キー
		KEY_F2,				// Ｆ２キー
		KEY_F3,				// Ｆ３キー
		KEY_F4,				// Ｆ４キー
		KEY_F5,				// Ｆ５キー
		KEY_F6,				// Ｆ６キー
		KEY_F7,				// Ｆ７キー
		KEY_F8,				// Ｆ８キー
		KEY_F9,				// Ｆ９キー
		KEY_F10,			// Ｆ１０キー
		KEY_F11,			// Ｆ１１キー
		KEY_F12,			// Ｆ１２キー

		KEY_A,				// Ａキー
		KEY_B,				// Ｂキー
		KEY_C,				// Ｃキー
		KEY_D,				// Ｄキー
		KEY_E,				// Ｅキー
		KEY_F,				// Ｆキー
		KEY_G,				// Ｇキー
		KEY_H,				// Ｈキー
		KEY_I,				// Ｉキー
		KEY_J,				// Ｊキー
		KEY_K,				// Ｋキー
		KEY_L,				// Ｌキー
		KEY_M,				// Ｍキー
		KEY_N,				// Ｎキー
		KEY_O,				// Ｏキー
		KEY_P,				// Ｐキー
		KEY_Q,				// Ｑキー
		KEY_R,				// Ｒキー
		KEY_S,				// Ｓキー
		KEY_T,				// Ｔキー
		KEY_U,				// Ｕキー
		KEY_V,				// Ｖキー
		KEY_W,				// Ｗキー
		KEY_X,				// Ｘキー
		KEY_Y,				// Ｙキー
		KEY_Z,				// Ｚキー

		KEY_0,				// ０キー
		KEY_1,				// １キー
		KEY_2,				// ２キー
		KEY_3,				// ３キー
		KEY_4,				// ４キー
		KEY_5,				// ５キー
		KEY_6,				// ６キー
		KEY_7,				// ７キー
		KEY_8,				// ８キー
		KEY_9,				// ９キー
	};

	//// マウスボタン列挙定数
	//enum class MouseCode {
	//	MOUSE_LEFT,		// マウス左ボタン
	//	MOUSE_RIGHT,	// マウス右ボタン
	//	MOUSE_MIDDLE,	// マウス中央ボタン
	//	MOUSE_1,		// マウス１ボタン
	//	MOUSE_2,		// マウス２ボタン
	//	MOUSE_3,		// マウス３ボタン
	//	MOUSE_4,		// マウス４ボタン
	//	MOUSE_5,		// マウス５ボタン
	//	MOUSE_6,		// マウス６ボタン
	//	MOUSE_7,		// マウス７ボタン
	//	MOUSE_8,		// マウス８ボタン
	//};

	class Input final {
		// initialize, finalizeを呼ぶため
		friend class Library;
	private:
		// 生成の禁止
		Input() = delete;
		Input(const Input&) = delete;
		Input(const Input&&) = delete;
		Input& operator =(const Input&) = delete;
		Input& operator =(const Input&&) = delete;

		inline static constexpr int KEY_MAX = 256;

		inline static LPDIRECTINPUT8 input_ = nullptr;
		inline static LPDIRECTINPUTDEVICE8 device_ = nullptr;
		inline static BYTE key_now_[KEY_MAX];
		inline static BYTE key_prev_[KEY_MAX];

		static bool initialize();
		static bool update();
		static bool finalize();

	public:
		//====================================================================================================
		// static関数

		// キー押下
		static bool isKeyDown(const KeyCode key_code);

		// キーが押された瞬間
		static bool isKeyDownTrigger(const KeyCode key_code);

		// キーが離された瞬間
		static bool isKeyReleaseTrigger(const KeyCode key_code);

		//====================================================================================================
	};

}
