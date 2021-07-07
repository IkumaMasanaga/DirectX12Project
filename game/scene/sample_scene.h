#pragma once
#include "../../engine/engine.h"


class SampleScene final : public eng::Scene {
public:
	using s_ptr = std::shared_ptr<SampleScene>;
	using w_ptr = std::weak_ptr<SampleScene>;
private:
	bool initialize() final override;
	void lateUpdate() final override;
	void finalize() final override;
public:
	SampleScene() {}
	~SampleScene() {}
};

