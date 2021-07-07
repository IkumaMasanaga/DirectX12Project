#include "time.h"


namespace lib {

	void Time::initialize() {
		start_ = std::chrono::system_clock::now();
	}

	void Time::update() {
		// フレーム間の経過時間
		// マイクロ秒で計測して秒に変換
		end_ = std::chrono::system_clock::now();
		long long microseconds = std::chrono::duration_cast<std::chrono::microseconds>(end_ - start_).count();
		delta_time_ = static_cast<float>(microseconds / 1000.0 / 1000.0);
		start_ = end_;
		elapsed_time_ += delta_time_;
		++elapsed_frame_;
		delta_time_avg_ = elapsed_time_ / elapsed_frame_;
		fps_ = static_cast<int>(elapsed_frame_ / elapsed_time_);
	}

}
