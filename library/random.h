#pragma once
#include <stdint.h>
#include <stdlib.h>


namespace lib {

	class Random final {
	private:
		// 生成の禁止
		Random() = delete;
		Random(const Random&) = delete;
		Random(const Random&&) = delete;
		Random& operator =(const Random&) = delete;
		Random& operator =(const Random&&) = delete;

		//====================================================================================================
		// static変数

		inline static uint32_t seed_ = 0;	// 乱数シード値

		//====================================================================================================
	public:
		//====================================================================================================
		// static関数

		// シード値の設定
		inline static void setSeed(const uint32_t seed) { seed_ = seed; srand(seed_); }

		// シード値の取得
		inline static uint32_t getSeed() { return seed_; }

		// 範囲内の乱数を取得
		template<class T>
		inline static T range(T min, T max) {
			return min + static_cast<T>((rand()) / (static_cast<T>(RAND_MAX / (max - min))));
		}

		//====================================================================================================
	};

}
