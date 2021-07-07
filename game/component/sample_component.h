#pragma once
#include "../../engine/engine.h"


class SampleComponent final : public eng::Component {
public:
	using s_ptr = std::shared_ptr<SampleComponent>;
	using w_ptr = std::weak_ptr<SampleComponent>;
private:

	void update() final override;

public:
	SampleComponent() {}
	~SampleComponent() {}

};
