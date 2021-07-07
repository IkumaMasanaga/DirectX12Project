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


namespace lib {

	class Library final {
		// initialize, update, finalize‚ğŒÄ‚Ô‚½‚ß
		friend class sys::System;
	private:
		// ¶¬‚Ì‹Ö~
		Library() = delete;
		Library(const Library&) = delete;
		Library(const Library&&) = delete;
		Library& operator = (const Library&) = delete;
		Library& operator = (const Library&&) = delete;

		//====================================================================================================
		// staticŠÖ”

		// ‰Šú‰»
		static bool initialize();

		// XV
		static bool update();

		// I—¹ˆ—
		static void finalize();

		//====================================================================================================
	};

}
