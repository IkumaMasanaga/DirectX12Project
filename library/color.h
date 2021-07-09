#pragma once


namespace lib {

	class Color final {
	public:
		Color() : r(0.0f), g(0.0f), b(0.0f), alpha(0.0f) {}
		Color(const float r, const float g, const float b, const float alpha) : r(r), g(g), b(b), alpha(alpha) {}
		~Color() {}

		union {
			struct {
				float r;
				float g;
				float b;
				float alpha;
			};
			float c[4];
		};

		inline void set(const float r, const float g, const float b, const float alpha) {
			this->r = r;
			this->g = g;
			this->b = b;
			this->alpha = alpha;
		}
	};

}
