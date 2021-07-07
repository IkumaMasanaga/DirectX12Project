#pragma once
#include <windows.h>


namespace sys {

	class System final {
	private:
		System() {}
		~System() {}

	public:
		//====================================================================================================
		// static•Ï”

		// ‰Šú‰»
		static bool initialize(HINSTANCE h_instance);

		// XV
		static void update();

		// I—¹ˆ—
		static void finalize();

		//====================================================================================================
	};

}
