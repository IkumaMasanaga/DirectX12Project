#pragma once


namespace lib {

	class Math final {
	private:
		// 生成の禁止
		Math() = delete;
		Math(const Math&) = delete;
		Math(const Math&&) = delete;
		Math& operator =(const Math&) = delete;
		Math& operator =(const Math&&) = delete;

	public:
		//====================================================================================================
		// static定数

		inline static const float PI = 3.1415965352f;	// 円周率

		//====================================================================================================
		// static関数

		// ラジアンから度へ変換
		inline static float toDegree(const float radian) { return radian * 180 / PI; }

		// 度からラジアンへ変換
		inline static float toRadian(const float degree) { return degree * PI / 180; }

		//====================================================================================================
	};

}
