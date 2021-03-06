#pragma once
#include "../system/system.h"

#include "math.h"
#include "vector2.h"
#include "vector3.h"
#include "quaternion.h"
#include "matrix4x4.h"
#include "random.h"
#include "bit_flag.h"
#include "link_linear.h"
#include "link_tree.h"
#include "singleton.h"
#include "smart_factory.h"
#include "shared_flyweight_map.h"
#include "time.h"
#include "fps_control.h"
#include "utility.h"
#include "debug.h"
#include "color.h"
#include "input.h"


namespace lib {

	class Library final {
		// initialize, update, finalizeを呼ぶため
		friend class sys::System;
	private:
		// 生成の禁止
		Library() = delete;
		Library(const Library&) = delete;
		Library(const Library&&) = delete;
		Library& operator = (const Library&) = delete;
		Library& operator = (const Library&&) = delete;

		//====================================================================================================
		// static関数

		// 初期化
		static bool initialize();

		// 更新
		static bool update();

		// 終了処理
		static void finalize();

		//====================================================================================================
	};

}
