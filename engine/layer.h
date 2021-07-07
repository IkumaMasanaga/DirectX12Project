#pragma once
#include "../library/library.h"


namespace eng {

	class Layer final : public lib::SmartFactory {
	public:
		using s_ptr = std::shared_ptr<Layer>;
		using w_ptr = std::weak_ptr<Layer>;
	private:


	public:
		Layer() {}
		~Layer() {}

	};

}
