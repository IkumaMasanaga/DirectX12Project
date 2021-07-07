#pragma once
#include <DirectXMath.h>


namespace lib {

	class Vector3;
	class Matrix4x4;

	class Quaternion final {
	public:
		Quaternion() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
		Quaternion(const float x, const float y, const float z, const float w) : x(x), y(y), z(z), w(w) {}
		Quaternion(const Vector3& v, const float w);
		Quaternion(const DirectX::XMFLOAT4& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
		Quaternion(const DirectX::XMVECTOR& v) : x(v.m128_f32[0]), y(v.m128_f32[1]), z(v.m128_f32[2]), w(v.m128_f32[3]) {}
		~Quaternion() {}

		//====================================================================================================
		// メンバ変数

		float x, y, z, w;

		//====================================================================================================
		// static定数

		static const Quaternion IDENTITY;	// 単位回転

		//====================================================================================================
		// メンバ関数

		// 各種演算子
		Quaternion operator +(const Quaternion& other) const;
		Quaternion& operator +=(const Quaternion& other);
		Quaternion operator -(const Quaternion& other) const;
		Quaternion& operator -=(const Quaternion& other);
		Quaternion operator *(const Quaternion& other) const;
		Quaternion& operator *=(const Quaternion& other);
		inline operator DirectX::XMFLOAT4() const { return DirectX::XMFLOAT4(x, y, z, w); }
		inline operator DirectX::XMVECTOR() const {
			DirectX::XMFLOAT4 v = *this;
			return DirectX::XMLoadFloat4(&v);
		}

		// オイラー角に変換
		Vector3 getEulerAngle() const;

		// 回転行列に変換
		Matrix4x4 getMatrix4x4() const;

		//====================================================================================================
		// static関数

		// 任意回転軸のクォータニオンを作成
		static Quaternion createRotationAxis(const Vector3& axis, const float angle);

		// オイラー角からクォータニオンを作成
		static Quaternion createEulerAngle(const float x, const float y, const float z);
		static Quaternion createEulerAngle(const Vector3& euler);

		// 回転行列からクォータニオンを作成
		static Quaternion createRotationMatrix4x4(const Matrix4x4& matrix);

		// 逆クォータニオンを作成
		static Quaternion createInverse(const Quaternion& q);

		//====================================================================================================
	};

}
