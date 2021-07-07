#include <Windows.h>
#pragma comment(lib, "winmm.lib")
#include "fps_control.h"


namespace lib {

	void FPSControl::update() {
		start_ = std::chrono::system_clock::now();
		int wait_time = static_cast<int>((1000 / fps_setting_) - (std::chrono::duration_cast<std::chrono::milliseconds>(start_ - end_).count()));
		if (0 < wait_time) {
			timeBeginPeriod(1);
			Sleep(wait_time);
			timeEndPeriod(1);
		}
		end_ = std::chrono::system_clock::now();
	}

}
