#pragma once
#include <windows.h>


namespace sys {

	class System final {
	private:
		System() {}
		~System() {}

	public:
		//====================================================================================================
		// static変数

		// 初期化
		static bool initialize(HINSTANCE h_instance);

		// 更新
		static void update();

		// 終了処理
		static void finalize();

		//====================================================================================================
	};

}
