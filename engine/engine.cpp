#include "engine.h"

//==================================================
// ‰ŠúƒV[ƒ“

#include "../game/scene/sample_scene.h"

//==================================================


namespace eng {

	bool Engine::initialize() {

		now_ = next_ = SampleScene::createShared<SampleScene>();
		now_->onCreated();

		return true;
	}

	void Engine::update() {

		if (now_ != next_) {
			now_->finalize();
			now_ = next_;
			now_->onCreated();
		}
		now_->update();
		now_->render();

	}

	void Engine::finalize() {

		now_->finalize();

	}

}
