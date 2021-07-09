#include "system.h"
#include "window.h"
#include "dx12_manager.h"
#include "../library/library.h"
#include "../engine/graphics/graphics_manager.h"
#include "../engine/engine.h"


namespace sys {

	bool System::initialize(HINSTANCE h_instance) {

		// ウィンドウの作成
		if (!sys::Window::createWindow(h_instance)) {
			sys::Window::warningMessageBox(L"ウィンドウの作成に失敗しました。");
			return false;
		}

		// DirectX12の初期化
		if (!sys::Dx12Manager::getInstance().initialize()) {
			sys::Window::warningMessageBox(L"DirectX12の初期化に失敗しました。");
			return false;
		}
			
		// 描画設定の初期化
		if (!eng::GraphicsManager::getInstance().initialize()) {
			sys::Window::warningMessageBox(L"描画設定の初期化に失敗しました。");
			return false;
		}

		// ライブラリの初期化
		if (!lib::Library::initialize()) {
			sys::Window::warningMessageBox(L"ライブラリの初期化に失敗しました。");
			return false;
		}
			
		// エンジンの初期化
		if (!eng::Engine::getInstance().initialize()) {
			sys::Window::warningMessageBox(L"エンジンの初期化に失敗しました。");
			return false;
		}

		// ウィンドウの表示
		ShowWindow(sys::Window::handle_, SW_SHOW);

		return true;
	}

	void System::update() {

		// ライブラリの更新
		lib::Library::update();

		// エンジンの更新
		eng::Engine::getInstance().update();

		// 描画完了の同期
		eng::GraphicsManager::getInstance().waitForPreviousFrame();

	}
	
	void System::finalize() {

		// エンジンの終了処理
		eng::Engine::getInstance().finalize();

		// ライブラリの終了処理
		lib::Library::finalize();

		// 描画の終了処理
		eng::GraphicsManager::getInstance().finalize();

		// DirectX12の終了処理
		sys::Dx12Manager::getInstance().finalize();
	}

}
