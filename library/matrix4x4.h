#pragma once
#include <DirectXMath.h>


namespace lib {

	class Vector3;

	class Matrix4x4 final {
	public:
		Matrix4x4() :
			_11(1.0f), _12(0.0f), _13(0.0f), _14(0.0f),
			_21(0.0f), _22(1.0f), _23(0.0f), _24(0.0f),
			_31(0.0f), _32(0.0f), _33(1.0f), _34(0.0f),
			_41(0.0f), _42(0.0f), _43(0.0f), _44(1.0f)
		{}
		Matrix4x4(
			const float _11, const float _12, const float _13, const float _14,
			const float _21, const float _22, const float _23, const float _24,
			const float _31, const float _32, const float _33, const float _34,
			const float _41, const float _42, const float _43, const float _44
		) :
			_11(_11), _12(_12), _13(_13), _14(_14),
			_21(_21), _22(_22), _23(_23), _24(_24),
			_31(_31), _32(_32), _33(_33), _34(_34),
			_41(_41), _42(_42), _43(_43), _44(_44)
		{}
		Matrix4x4(const DirectX::XMMATRIX& matrix) :
			_11(matrix.r[0].m128_f32[0]), _12(matrix.r[0].m128_f32[1]), _13(matrix.r[0].m128_f32[2]), _14(matrix.r[0].m128_f32[3]),
			_21(matrix.r[1].m128_f32[0]), _22(matrix.r[1].m128_f32[1]), _23(matrix.r[1].m128_f32[2]), _24(matrix.r[1].m128_f32[3]),
			_31(matrix.r[2].m128_f32[0]), _32(matrix.r[2].m128_f32[1]), _33(matrix.r[2].m128_f32[2]), _34(matrix.r[2].m128_f32[3]),
			_41(matrix.r[3].m128_f32[0]), _42(matrix.r[3].m128_f32[1]), _43(matrix.r[3].m128_f32[2]), _44(matrix.r[3].m128_f32[3])
		{}
		Matrix4x4(const DirectX::XMFLOAT4X4& matrix) :
			_11(matrix._11), _12(matrix._12), _13(matrix._13), _14(matrix._14),
			_21(matrix._21), _22(matrix._22), _23(matrix._23), _24(matrix._24),
			_31(matrix._31), _32(matrix._32), _33(matrix._33), _34(matrix._34),
			_41(matrix._41), _42(matrix._42), _43(matrix._43), _44(matrix._44)
		{}
		~Matrix4x4() {}

		//====================================================================================================
		// メンバ変数

		union {
			struct {
				float _11, _12, _13, _14;
				float _21, _22, _23, _24;
				float _31, _32, _33, _34;
				float _41, _42, _43, _44;
			};
			float m[16];
		};

		//====================================================================================================
		// static定数

		static const Matrix4x4 IDENTITY;	// 単位行列

		//====================================================================================================
		// メンバ関数

		// 各種演算子オーバーロード
		// 順番を間違えると結果が変わるので注意
		Matrix4x4 operator +(const Matrix4x4& other) const;
		Matrix4x4& operator +=(const Matrix4x4& other);
		Matrix4x4 operator -(const Matrix4x4& other) const;
		Matrix4x4& operator -=(const Matrix4x4& other);
		Matrix4x4 operator *(const Matrix4x4& other) const;
		Matrix4x4 operator *(const float scale) const;
		Matrix4x4& operator *=(const Matrix4x4& other);
		Matrix4x4& operator *=(const float scale);
		inline operator DirectX::XMMATRIX() const { return DirectX::XMMATRIX(m); }
		inline operator DirectX::XMFLOAT4X4() const { return DirectX::XMFLOAT4X4(m); }

		//====================================================================================================
		// static関数

		// 平行移動行列の生成
		static Matrix4x4 createTranslation(const float x, const float y, const float z);
		static Matrix4x4 createTranslation(const Vector3& vector);

		// 回転軸指定の行列の生成
		static Matrix4x4 createRotationAxis(const Vector3& axis, const float angle);

		// 拡大縮小行列の生成
		static Matrix4x4 createScaling(const float x, const float y, const float z);
		static Matrix4x4 createScaling(const Vector3& vector);

		// 行列を元にベクトルを生成
		static Vector3 createTransformCoord(const Vector3& vector, const Matrix4x4& matrix);

		// 転置行列の生成
		static Matrix4x4 createTranspose(const Matrix4x4& matrix);

		//====================================================================================================
	};

}
