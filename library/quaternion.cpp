#include "quaternion.h"
#include "vector3.h"
#include "matrix4x4.h"
#include "math.h"


namespace lib {

	const Quaternion Quaternion::IDENTITY = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);

	Quaternion::Quaternion(const Vector3& v, const float w) : x(v.x), y(v.y), z(v.z), w(w) {}

	Quaternion Quaternion::operator +(const Quaternion& other) const {
		return Quaternion(x + other.x, y + other.y, z + other.z, w + other.w);
	}

	Quaternion& Quaternion::operator +=(const Quaternion& other) {
		*this = *this + other;
		return *this;
	}

	Quaternion Quaternion::operator -(const Quaternion& other) const {
		Quaternion q = createInverse(*this);
		q *= other;
		if (q.w < 0) {
			q.x = -q.x;
			q.y = -q.y;
			q.z = -q.z;
			q.w = -q.w;
		}
		return q;
	}

	Quaternion& Quaternion::operator -=(const Quaternion& other) {
		*this = *this - other;
		return *this;
	}
	
	Quaternion Quaternion::operator *(const Quaternion& other) const {
		return Quaternion(
			(x * other.w) + (w * other.x) + (y * other.z) - (z * other.y),
			(y * other.w) + (w * other.y) + (z * other.x) - (x * other.z),
			(z * other.w) + (w * other.z) + (x * other.y) - (y * other.x),
			(w * other.w) - (x * other.x) - (y * other.y) - (z * other.z)
		);
	}

	Quaternion& Quaternion::operator *=(const Quaternion& other) {
		*this = *this * other;
		return *this;
	}

	Vector3 Quaternion::getEulerAngle() const {
		float x2 = x * x;
		float y2 = y * y;
		float z2 = z * z;

		float xy = x * y;
		float xz = x * z;
		float yz = y * z;
		float wx = w * x;
		float wy = w * y;
		float wz = w * z;

		// 1 - 2y^2 - 2z^2
		float m00 = 1.f - (2.f * y2) - (2.f * z2);

		// 2xy + 2wz
		float m01 = (2.f * xy) + (2.f * wz);

		// 2xy - 2wz
		float m10 = (2.f * xy) - (2.f * wz);

		// 1 - 2x^2 - 2z^2
		float m11 = 1.f - (2.f * x2) - (2.f * z2);

		// 2xz + 2wy
		float m20 = (2.f * xz) + (2.f * wy);

		// 2yz+2wx
		float m21 = (2.f * yz) - (2.f * wx);

		// 1 - 2x^2 - 2y^2
		float m22 = 1.f - (2.f * x2) - (2.f * y2);

		float tx, ty, tz;

		if (fabsf(m21 - 1.0f) < FLT_EPSILON)
		{
			tx = Math::PI / 2.f;
			ty = 0;
			tz = atan2f(m10, m00);
		}
		else if (fabsf(m21 + 1.0f) < FLT_EPSILON)
		{
			tx = -Math::PI / 2.0f;
			ty = 0;
			tz = atan2f(m10, m00);
		}
		else
		{
			tx = asinf(-m21);
			ty = atan2f(m20, m22);
			tz = atan2f(m01, m11);
		}

		return Vector3(tx, ty, tz);
	}

	Matrix4x4 Quaternion::getMatrix4x4() const {
		return Matrix4x4(
			1.0f - 2.0f*y*y - 2.0f*z*z, 2.0f*x*y + 2.0f*w*z,        2.0f*x*z - 2.0f*w*y,        0.0f,
			2.0f*x*y - 2.0f*w*z,        1.0f - 2.0f*x*x - 2.0f*z*z, 2.0f*y*z + 2.0f*w*x,        0.0f,
			2.0f*x*z + 2.0f*w*y,        2.0f*y*z - 2.0f*w*x,        1.0f - 2.0f*x*x - 2.0f*y*y, 0.0f,
			0.0f,                       0.0f,                       0.0f,                       1.0f
		);
	}

	Quaternion Quaternion::createRotationAxis(const Vector3& axis, const float angle) {
		float sf = sinf(angle / 2.0f);
		return Quaternion(
			axis.x * sf,
			axis.y * sf,
			axis.z * sf,
			cosf(angle / 2.0f)
		);
	}

	Quaternion Quaternion::createEulerAngle(const float x, const float y, const float z) {
		return createRotationAxis(Vector3::FORWARD, z) * createRotationAxis(Vector3::RIGHT, x) * createRotationAxis(Vector3::UP, y);
	}

	Quaternion Quaternion::createEulerAngle(const Vector3& euler) {
		return createEulerAngle(euler.x, euler.y, euler.z);
	}

	Quaternion Quaternion::createRotationMatrix4x4(const Matrix4x4& matrix) {

		Quaternion ret;
		
		// 最大成分を検索
		float elem[4]; // 0:x, 1:y, 2:z, 3:w
		elem[0] = matrix._11 - matrix._22 - matrix._33 + 1.0f;
		elem[1] = -matrix._11 + matrix._22 - matrix._33 + 1.0f;
		elem[2] = -matrix._11 - matrix._22 + matrix._33 + 1.0f;
		elem[3] = matrix._11 + matrix._22 + matrix._33 + 1.0f;

		unsigned biggestIndex = 0;
		for (int i = 1; i < 4; i++) {
			if (elem[i] > elem[biggestIndex])
				biggestIndex = i;
		}

		if (elem[biggestIndex] < 0.0f)
			return ret; // 引数の行列に間違いあり！

		// 最大要素の値を算出
		float* q[4] = { &ret.x, &ret.y, &ret.z, &ret.w };
		float v = sqrtf(elem[biggestIndex]) * 0.5f;
		*q[biggestIndex] = v;
		float mult = 0.25f / v;

		switch (biggestIndex) {
		case 0: // x
			*q[1] = (matrix._12 + matrix._21) * mult;
			*q[2] = (matrix._31 + matrix._13) * mult;
			*q[3] = (matrix._23 - matrix._32) * mult;
			break;
		case 1: // y
			*q[0] = (matrix._12 + matrix._21) * mult;
			*q[2] = (matrix._23 + matrix._32) * mult;
			*q[3] = (matrix._31 - matrix._13) * mult;
			break;
		case 2: // z
			*q[0] = (matrix._31 + matrix._13) * mult;
			*q[1] = (matrix._23 + matrix._32) * mult;
			*q[3] = (matrix._12 - matrix._21) * mult;
			break;
		case 3: // w
			*q[0] = (matrix._23 - matrix._32) * mult;
			*q[1] = (matrix._31 - matrix._13) * mult;
			*q[2] = (matrix._12 - matrix._21) * mult;
			break;
		}

		return ret;
	}

	Quaternion Quaternion::createInverse(const Quaternion& q) {
		return Quaternion(-q.x, -q.y, -q.z, q.w);
	}

}
