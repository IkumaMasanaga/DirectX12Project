#pragma once
#include <chrono>


namespace lib {

	class Time final {
		friend class Library;
	private:
		// 生成の禁止
		Time() = delete;
		Time(const Time&) = delete;
		Time(const Time&&) = delete;
		Time& operator =(const Time&) = delete;
		Time& operator =(const Time&&) = delete;

		//====================================================================================================
		// static変数

		inline static std::chrono::system_clock::time_point start_;	// 前回フレームの時間
		inline static std::chrono::system_clock::time_point end_;	// 現在フレームの時間
		inline static float delta_time_ = 0.0f;						// 1フレーム当たりの時間
		inline static float delta_time_avg_ = 0.0f;					// 1フレーム当たりの時間の平均
		inline static float elapsed_time_ = 0.0f;					// ゲームを起動してからの経過時間
		inline static int fps_ = 0;									// 1秒当たりのフレーム数
		inline static int elapsed_frame_ = 0;						// ゲームを起動してからの経過フレーム数

		//====================================================================================================
		// static関数

		// 初期化
		static void initialize();

		// 更新
		static void update();

		//====================================================================================================
	public:
		//====================================================================================================
		// static関数

		// ゲッター
		inline static float getDeltaTime() { return delta_time_; }
		inline static float getDeltaTimeAvg() { return delta_time_avg_; }
		inline static float getElapsedTime() { return elapsed_time_; }
		inline static int getFPS() { return fps_; }
		inline static int getElapsedFrame() { return elapsed_frame_; }

		//====================================================================================================
	};

}
