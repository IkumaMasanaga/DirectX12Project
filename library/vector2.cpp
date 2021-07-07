#include "vector2.h"
#include "vector3.h"


namespace lib {

	const Vector2 Vector2::ZERO		= {  0,  0 };
	const Vector2 Vector2::ONE		= {  1,  1 };
	const Vector2 Vector2::UP		= {  0,  1 };
	const Vector2 Vector2::DOWN		= {  0, -1 };
	const Vector2 Vector2::LEFT		= { -1,  0 };
	const Vector2 Vector2::RIGHT	= {  1,  0 };

	Vector2 Vector2::operator +(const Vector2& other) const {
		return Vector2(x + other.x, y + other.y);
	}

	Vector2& Vector2::operator +=(const Vector2& other) {
		*this = *this + other;
		return *this;
	}

	Vector2 Vector2::operator -() const {
		return Vector2(-x, -y);
	}

	Vector2 Vector2::operator -(const Vector2& other) const {
		return Vector2(x - other.x, y - other.y);
	}

	Vector2& Vector2::operator -=(const Vector2& other) {
		*this = *this - other;
		return *this;
	}

	Vector2 Vector2::operator *(const Vector2& other) const {
		return Vector2(x * other.x, y * other.y);
	}

	Vector2 Vector2::operator *(const float other) const {
		return Vector2(x * other, y * other);
	}

	Vector2& Vector2::operator *=(const Vector2& other) {
		*this = *this * other;
		return *this;
	}

	Vector2& Vector2::operator *=(const float other) {
		*this = *this * other;
		return *this;
	}

	Vector2 Vector2::operator /(const Vector2& other) const {
		return Vector2((other.x == 0.0f) ? 0.0f : x / other.x, (other.y == 0.0f) ? 0.0f : y / other.y);
	}

	Vector2 Vector2::operator /(const float other) const {
		return (other == 0.0f) ? Vector2::ZERO : Vector2(x / other, y / other);
	}

	Vector2& Vector2::operator /=(const Vector2& other) {
		*this = *this / other;
		return *this;
	}

	Vector2& Vector2::operator /=(const float other) {
		*this = *this / other;
		return *this;
	}

	bool Vector2::operator ==(const Vector2& other) const {
		return x == other.x && y == other.y;
	}

	bool Vector2::operator !=(const Vector2& other) const {
		return !(*this == other);
	}

	Vector2::operator Vector3() const {
		return Vector3(x, y, 0.0f);
	}

	float Vector2::length() const {
		return sqrtf(x * x + y * y);
	}

	float Vector2::sqrLength() const {
		return x * x + y * y;
	}

	Vector2 Vector2::normalize(const Vector2& v) {
		float l = v.length();
		l = (l > 0.0f) ? 1.0f / l : l;
		return Vector2(v.x * l, v.y * l);
	}

	float Vector2::angle(const Vector2& from, const Vector2& to) {
		Vector2 v1 = Vector2::normalize(from);
		Vector2 v2 = Vector2::normalize(to);
		return acosf(dot(v1, v2));
	}

	float Vector2::dot(const Vector2& v1, const Vector2& v2) {
		return v1.x * v2.x + v1.y * v2.y;
	}

	float Vector2::cross(const Vector2& v1, const Vector2& v2) {
		return v1.x * v2.y - v1.y * v2.x;
	}

	float Vector2::distance(const Vector2& v1, const Vector2& v2) {
		return (v1 - v2).length();
	}

	//Vector2 Vector2::min(const Vector2& v1, const Vector2& v2) {
	//	return Vector2(fminf(v1.x, v2.x), fminf(v1.y, v2.y));
	//}

	//Vector2 Vector2::max(const Vector2& v1, const Vector2& v2) {
	//	return Vector2(fmaxf(v1.x, v2.x), fmaxf(v1.y, v2.y));
	//}

}
