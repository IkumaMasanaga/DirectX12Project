#pragma once
#include <windows.h>
#include <d3d12.h>


namespace sys {

	class Window final {
		friend class System;
	private:
		Window() {}
		~Window() {}

		//====================================================================================================
		// static関数

		// ウィンドウプロシージャ
		static LRESULT CALLBACK procedure(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam);

		// ウィンドウの作成
		static HWND createWindow(HINSTANCE h_instance);

		//====================================================================================================
	public:
		//====================================================================================================
		// static定数

		// 幅
		inline static constexpr LONG WIDTH = 1280;

		// 高さ
		inline static constexpr LONG HEIGHT = 720;

		// ビューポート
		// オフスクリーンレンダリングを使用し画面分割する際に個別に必要？
		inline static constexpr D3D12_VIEWPORT VIEWPORT = { 0.0f, 0.0f, (FLOAT)WIDTH, (FLOAT)HEIGHT, 0.0f, 1.0f };
		inline static constexpr D3D12_RECT SCISSOR_RECT = { 0, 0, WIDTH, HEIGHT };

		inline static constexpr float ASPECT_RATIO = (float)WIDTH / (float)HEIGHT;

		//====================================================================================================
		// static変数

		// ハンドル
		inline static HWND handle_ = 0;

		//====================================================================================================
		// static関数

		// 警告メッセージボックスの表示
		static void warningMessageBox(const LPCWSTR& message);

		//====================================================================================================
	};

}
