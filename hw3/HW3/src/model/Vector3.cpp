#include "Vector3.h"

Vector3::Vector3(float x, float y, float z) : _x(x), _y(y), _z(z) {}

Vector3::Vector3() : Vector3(0.0f, 0.0f, 0.0f) {}

float Vector3::x() const { return _x; }
float Vector3::y() const { return _y; }
float Vector3::z() const { return _z; }

void Vector3::operator=(const Vector3& v)
{
	_x = v._x;
	_y = v._y;
	_z = v._z;
}

void Vector3::operator+=(const Vector3& v2)
{
	_x += v2._x;
	_y += v2._y;
	_z += v2._z;
}

Vector3 Vector3::operator*(float f) const {	return Vector3(_x * f, _y * f, _z * f); }
Vector3 Vector3::operator+(const Vector3& v2) const { return Vector3(_x + v2._x, _y + v2._y, _z + v2._z); }
Vector3 Vector3::operator-() const { return Vector3(-_x, -_y, -_z); }
Vector3 Vector3::operator-(const Vector3& v2) const { return Vector3(_x - v2._x, _y - v2._y, _z - v2._z); }
Vector3 operator*(float f, const Vector3& v) { return v * f; }

float Vector3::dot(const Vector3& v2) const
{
	return _x * v2._x + _y * v2._y + _z * v2._z;
}

Vector3 Vector3::cross(const Vector3& v2) const
{
	return Vector3(
		_y * v2._z - _z * v2._y,
		_z * v2._x - _x * v2._z,
		_x * v2._y - _y * v2._x
	);
}

Vector3 Vector3::normalize() const
{
	float magnitude = (float)sqrt(_x * (double)_x + _y * (double)_y + _z * (double)_z);
	return Vector3(
		_x / magnitude,
		_y / magnitude,
		_z / magnitude);
}

glm::mat3 Vector3::outerProduct(const Vector3& v2) const
{
	return glm::mat3( // In column-major
		_x * v2._x, _y * v2._x, _z * v2._x,
		_x * v2._y, _y * v2._y, _z * v2._y,
		_x * v2._z, _y * v2._z, _z * v2._z);
}
