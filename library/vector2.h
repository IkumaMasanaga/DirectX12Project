#pragma once
#include <DirectXMath.h>


namespace lib {

	class Vector3;

	class Vector2 final {
	public:
		Vector2() : x(0.0f), y(0.0f) {}
		Vector2(const float x, const float y) : x(x), y(y) {}
		Vector2(const DirectX::XMFLOAT2& v) : x(v.x), y(v.y) {}
		Vector2(const DirectX::XMVECTOR& v) : x(v.m128_f32[0]), y(v.m128_f32[1]) {}
		~Vector2() {}

		//====================================================================================================
		// メンバ変数

		float x, y;

		//====================================================================================================
		// static定数

		static const Vector2 ZERO;	// {  0,  0 }
		static const Vector2 ONE;	// {  1,  1 }
		static const Vector2 UP;	// {  0,  1 }
		static const Vector2 DOWN;	// {  0, -1 }
		static const Vector2 LEFT;	// { -1,  0 }
		static const Vector2 RIGHT;	// {  1,  0 }

		//====================================================================================================
		// メンバ関数

		// 各種演算子
		Vector2 operator +(const Vector2& other) const;
		Vector2& operator +=(const Vector2& other);
		Vector2 operator -() const;
		Vector2 operator -(const Vector2& other) const;
		Vector2& operator -=(const Vector2& other);
		Vector2 operator *(const Vector2& other) const;
		Vector2 operator *(const float other) const;
		Vector2& operator *=(const Vector2& other);
		Vector2& operator *=(const float other);
		Vector2 operator /(const Vector2& other) const;
		Vector2 operator /(const float other) const;
		Vector2& operator /=(const Vector2& other);
		Vector2& operator /=(const float other);
		bool operator ==(const Vector2& other) const;
		bool operator !=(const Vector2& other) const;
		operator Vector3() const;
		inline operator DirectX::XMFLOAT2() const { return DirectX::XMFLOAT2(x, y); }
		inline operator DirectX::XMVECTOR() const {
			DirectX::XMFLOAT2 v = *this;
			return DirectX::XMLoadFloat2(&v);
		}

		// セット
		inline void set(const float x, const float y) { this->x = x; this->y = y; }

		// 長さ
		float length() const;

		// 2乗の長さ
		float sqrLength() const;

		//====================================================================================================
		// static関数

		// 正規化
		static Vector2 normalize(const Vector2& v);

		// 角度
		static float angle(const Vector2& from, const Vector2& to);

		// 内積
		static float dot(const Vector2& v1, const Vector2& v2);

		// 外積
		static float cross(const Vector2& v1, const Vector2& v2);

		// 距離
		static float distance(const Vector2& v1, const Vector2& v2);

		//// 小さいほうの要素を抜き出して新しいベクトルを生成
		//static Vector2 min(const Vector2& v1, const Vector2& v2);

		//// 大きいほうの要素を抜き出して新しいベクトルを生成
		//static Vector2 max(const Vector2& v1, const Vector2& v2);

		//====================================================================================================
	};

}
