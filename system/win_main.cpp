#include "system.h"


// エントリーポイント
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd) {

	// 初期化
	if (!sys::System::initialize(hInstance)) return 0;

	// メッセージループ
	MSG	msg;
	while (true) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) break;
			
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	// 終了処理
	sys::System::finalize();

	return 0;
}
