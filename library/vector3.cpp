#include "vector3.h"
#include "vector2.h"


namespace lib {

	const Vector3 Vector3::ZERO		= {  0,  0,  0 };
	const Vector3 Vector3::ONE		= {  1,  1,  1 };
	const Vector3 Vector3::UP		= {  0,  1,  0 };
	const Vector3 Vector3::DOWN		= {  0, -1,  0 };
	const Vector3 Vector3::LEFT		= { -1,  0,  0 };
	const Vector3 Vector3::RIGHT	= {  1,  0,  0 };
	const Vector3 Vector3::FORWARD	= {  0,  0,  1 };
	const Vector3 Vector3::BACK		= {  0,  0, -1 };

	Vector3 Vector3::operator +(const Vector3& other) const {
		return Vector3(x + other.x, y + other.y, z + other.z);
	}

	Vector3& Vector3::operator +=(const Vector3& other) {
		*this = *this + other;
		return *this;
	}

	Vector3 Vector3::operator -() const {
		return Vector3(-x, -y, -z);
	}

	Vector3 Vector3::operator -(const Vector3& other) const {
		return Vector3(x - other.x, y - other.y, z - other.z);
	}

	Vector3& Vector3::operator -=(const Vector3& other) {
		*this = *this - other;
		return *this;
	}

	Vector3 Vector3::operator *(const Vector3& other) const {
		return Vector3(x * other.x, y * other.y, z * other.z);
	}

	Vector3 Vector3::operator *(const float other) const {
		return Vector3(x * other, y * other, z * other);
	}

	Vector3& Vector3::operator *=(const Vector3& other) {
		*this = *this * other;
		return *this;
	}

	Vector3& Vector3::operator *=(const float other) {
		*this = *this * other;
		return *this;
	}

	Vector3 Vector3::operator /(const Vector3& other) const {
		return Vector3((other.x == 0.0f) ? 0.0f : x / other.x, (other.y == 0.0f) ? 0.0f : y / other.y, (other.z == 0.0f) ? 0.0f : z / other.z);
	}

	Vector3 Vector3::operator /(const float other) const {
		return (other == 0.0f) ? Vector3::ZERO : Vector3(x / other, y / other, z / other);
	}

	Vector3& Vector3::operator /=(const Vector3& other) {
		*this = *this / other;
		return *this;
	}

	Vector3& Vector3::operator /=(const float other) {
		*this = *this / other;
		return *this;
	}

	bool Vector3::operator ==(const Vector3& other) const {
		return (x == other.x) && (y == other.y) && (z == other.z);
	}

	bool Vector3::operator !=(const Vector3& other) const {
		return !(*this == other);
	}

	Vector3::operator Vector2() const {
		return Vector2(x, y);
	}

	float Vector3::length() const {
		return sqrtf(x * x + y * y + z * z);
	}

	float Vector3::sqrLength() const {
		return x * x + y * y + z * z;
	}

	Vector3 Vector3::normalize(const Vector3& v) {
		float l = v.length();
		l = (l > 0.0f) ? 1.0f / l : l;
		return Vector3(v.x * l, v.y * l, v.z * l);
	}

	float Vector3::angle(const Vector3& from, const Vector3& to) {
		Vector3 v1 = Vector3::normalize(from);
		Vector3 v2 = Vector3::normalize(to);
		return acosf(dot(v1, v2));
	}

	float Vector3::dot(const Vector3& v1, const Vector3& v2) {
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	Vector3 Vector3::cross(const Vector3& v1, const Vector3& v2) {
		return Vector3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
	}

	float Vector3::distance(const Vector3& v1, const Vector3& v2) {
		return (v1 - v2).length();
	}

	//Vector3 Vector3::min(const Vector3& v1, const Vector3& v2) {
	//	return Vector3(fminf(v1.x, v2.x), fminf(v1.y, v2.y), fminf(v1.z, v2.z));
	//}

	//Vector3 Vector3::max(const Vector3& v1, const Vector3& v2) {
	//	return Vector3(fmaxf(v1.x, v2.x), fmaxf(v1.y, v2.y), fmaxf(v1.z, v2.z));
	//}

}
