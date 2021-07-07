#include "matrix4x4.h"
#include "vector3.h"


namespace lib {

	const Matrix4x4 Matrix4x4::IDENTITY = Matrix4x4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
	
	Matrix4x4 Matrix4x4::operator +(const Matrix4x4& other) const {
		return Matrix4x4(
			_11 + other._11, _12 + other._12, _13 + other._13, _14 + other._14,
			_21 + other._21, _22 + other._22, _23 + other._23, _24 + other._24,
			_31 + other._31, _32 + other._32, _33 + other._33, _34 + other._34,
			_41 + other._41, _42 + other._42, _43 + other._43, _44 + other._44
		);
	}

	Matrix4x4& Matrix4x4::operator +=(const Matrix4x4& other) {
		*this = *this + other;
		return *this;
	}

	Matrix4x4 Matrix4x4::operator -(const Matrix4x4& other) const {
		return Matrix4x4(
			_11 - other._11, _12 - other._12, _13 - other._13, _14 - other._14,
			_21 - other._21, _22 - other._22, _23 - other._23, _24 - other._24,
			_31 - other._31, _32 - other._32, _33 - other._33, _34 - other._34,
			_41 - other._41, _42 - other._42, _43 - other._43, _44 - other._44
		);
	}

	Matrix4x4& Matrix4x4::operator -=(const Matrix4x4& other) {
		*this = *this - other;
		return *this;
	}

	Matrix4x4 Matrix4x4::operator *(const Matrix4x4& other) const {
		return Matrix4x4(
			(_11 * other._11) + (_12 * other._21) + (_13 * other._31) + (_14 * other._41),
			(_11 * other._12) + (_12 * other._22) + (_13 * other._32) + (_14 * other._42),
			(_11 * other._13) + (_12 * other._23) + (_13 * other._33) + (_14 * other._43),
			(_11 * other._14) + (_12 * other._24) + (_13 * other._34) + (_14 * other._44),

			(_21 * other._11) + (_22 * other._21) + (_23 * other._31) + (_24 * other._41),
			(_21 * other._12) + (_22 * other._22) + (_23 * other._32) + (_24 * other._42),
			(_21 * other._13) + (_22 * other._23) + (_23 * other._33) + (_24 * other._43),
			(_21 * other._14) + (_22 * other._24) + (_23 * other._34) + (_24 * other._44),

			(_31 * other._11) + (_32 * other._21) + (_33 * other._31) + (_34 * other._41),
			(_31 * other._12) + (_32 * other._22) + (_33 * other._32) + (_34 * other._42),
			(_31 * other._13) + (_32 * other._23) + (_33 * other._33) + (_34 * other._43),
			(_31 * other._14) + (_32 * other._24) + (_33 * other._34) + (_34 * other._44),

			(_41 * other._11) + (_42 * other._21) + (_43 * other._31) + (_44 * other._41),
			(_41 * other._12) + (_42 * other._22) + (_43 * other._32) + (_44 * other._42),
			(_41 * other._13) + (_42 * other._23) + (_43 * other._33) + (_44 * other._43),
			(_41 * other._14) + (_42 * other._24) + (_43 * other._34) + (_44 * other._44)
		);
	}

	Matrix4x4 Matrix4x4::operator *(const float scale) const {
		return Matrix4x4(
			_11 * scale, _12 * scale, _13 * scale, _14 * scale,
			_21 * scale, _22 * scale, _23 * scale, _24 * scale,
			_31 * scale, _32 * scale, _33 * scale, _34 * scale,
			_41 * scale, _42 * scale, _43 * scale, _44 * scale
		);
	}

	Matrix4x4& Matrix4x4::operator *=(const Matrix4x4& other) {
		*this = *this * other;
		return *this;
	}

	Matrix4x4& Matrix4x4::operator *=(const float scale) {
		*this = *this * scale;
		return *this;
	}

	Matrix4x4 Matrix4x4::createTranslation(const float x, const float y, const float z) {
		return Matrix4x4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			   x,    y,    z, 1.0f
		);
	}

	Matrix4x4 Matrix4x4::createTranslation(const Vector3& vector) {
		return createTranslation(vector.x, vector.y, vector.z);
	}

	Matrix4x4 Matrix4x4::createRotationAxis(const Vector3& axis, const float angle) {
		float x = axis.x;
		float y = axis.y;
		float z = axis.z;
		float radians = angle;
		float cos = cosf(radians);
		float cosp = 1.0f - cos;
		float sin = sinf(radians);

		return Matrix4x4(
			cos + cosp * x * x,     cosp * x * y + z * sin, cosp * x * z - y * sin, 0.0f,
			cosp * x * y - z * sin, cos + cosp * y * y,     cosp * y * z + x * sin, 0.0f,
			cosp * x * z + y * sin, cosp * y * z - x * sin, cos + cosp * z * z,     0.0f,
			0.0f,                   0.0f,                   0.0f,                   1.0f
		);
	}

	Matrix4x4 Matrix4x4::createScaling(const float x, const float y, const float z) {
		return Matrix4x4(
			   x, 0.0f, 0.0f, 0.0f,
			0.0f,    y, 0.0f, 0.0f,
			0.0f, 0.0f,    z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	}

	Matrix4x4 Matrix4x4::createScaling(const Vector3& vector) {
		return createScaling(vector.x, vector.y, vector.z);
	}

	Vector3 Matrix4x4::createTransformCoord(const Vector3& vector, const Matrix4x4& matrix) {
		Matrix4x4 mtx;
		mtx = createTranslation(vector.x, vector.y, vector.z);
		mtx = matrix * mtx;
		return Vector3(mtx._41, mtx._42, mtx._43);
	}

	Matrix4x4 Matrix4x4::createTranspose(const Matrix4x4& matrix) {
		return Matrix4x4(
			matrix._11, matrix._21, matrix._31, matrix._41,
			matrix._12, matrix._22, matrix._32, matrix._42,
			matrix._13, matrix._23, matrix._33, matrix._43,
			matrix._14, matrix._24, matrix._34, matrix._44
		);
	}

}
