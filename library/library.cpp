#include "library.h"


namespace lib {

	bool Library::initialize() {
		
		Random::setSeed(static_cast<uint32_t>(time(0)));
		FPSControl::setFPS(60);
		FPSControl::end_ = std::chrono::system_clock::now();
		Time::initialize();

		return true;
	}

	bool Library::update() {

		FPSControl::update();
		Time::update();

		return true;
	}

	void Library::finalize() {



	}

}
