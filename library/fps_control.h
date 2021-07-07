#pragma once
#include <chrono>


namespace lib {

	class FPSControl final {
		// updateを呼ぶため
		friend class Library;
	private:
		// 生成の禁止
		FPSControl() = delete;
		FPSControl(const FPSControl&) = delete;
		FPSControl(const FPSControl&&) = delete;
		FPSControl& operator =(const FPSControl&) = delete;
		FPSControl& operator =(const FPSControl&&) = delete;

		//====================================================================================================
		// static変数

		inline static int fps_setting_ = 60;						// 設定されたリフレッシュレート
		inline static std::chrono::system_clock::time_point start_;	// コントロール前の時間
		inline static std::chrono::system_clock::time_point end_;	// コントロール後の時間

		//====================================================================================================
		// static関数

		// 更新
		static void update();

		//====================================================================================================
	public:
		//====================================================================================================

		// セッター
		inline static void setFPS(const int fps) { fps_setting_ = fps; }

		// ゲッター
		inline static int getFPSSetting() { return fps_setting_; }

		//====================================================================================================
	};

}
