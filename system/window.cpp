#include "window.h"
#include "system.h"


namespace sys {

	LRESULT CALLBACK Window::procedure(HWND h_wnd, UINT n_msg, WPARAM w_param, LPARAM l_param) {
		switch (n_msg) {
		case WM_PAINT:
			System::update();
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(h_wnd, n_msg, w_param, l_param);
		}

		return 0;
	}

	HWND Window::createWindow(HINSTANCE h_instance) {

		// スタイル
		const LPCWSTR WINDOW_CLASS = L"DirectX12Project";
		const LPCWSTR WINDOW_TITLE = WINDOW_CLASS;
		const LONG WINDOW_STYLE = WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX;

		// ウィンドウクラスを登録
		WNDCLASSEX wndclass = {};
		wndclass.cbSize = sizeof(WNDCLASSEX);
		wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
		wndclass.lpfnWndProc = procedure;
		wndclass.hInstance = h_instance;
		wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndclass.lpszClassName = WINDOW_CLASS;
		RegisterClassEx(&wndclass);

		RECT windowRect = { 0, 0, WIDTH, HEIGHT };

		AdjustWindowRect(&windowRect, WINDOW_STYLE, FALSE);

		// ウィンドウを作成
		handle_ = CreateWindow(
			WINDOW_CLASS,
			WINDOW_TITLE,
			WINDOW_STYLE,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			windowRect.right - windowRect.left,
			windowRect.bottom - windowRect.top,
			NULL,
			NULL,
			h_instance,
			NULL);

		return handle_;
	}

	void Window::warningMessageBox(const LPCWSTR& message) {
		MessageBox(handle_, message, L"sys::Window::warningMessageBox", MB_OK | MB_ICONEXCLAMATION);
	}

}
