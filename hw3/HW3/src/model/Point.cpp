#include "Point.h"

Point::Point(float x, float y, float z) : _x(x), _y(y), _z(z) {}
Point::Point(glm::vec3 p) : _x(p.x), _y(p.y), _z(p.z) {}
Point::Point(glm::vec4 p) : _x(p.x / p.w), _y(p.y / p.w), _z(p.z / p.w) {}
Point::Point() : Point(0.0f, 0.0f, 0.0f) {}

void Point::operator+=(const Vector3& v)
{
	_x += v.x();
	_y += v.y();
	_z += v.z();
}

Vector3 Point::operator-(const Point& p) const { return Vector3(_x - p._x, _y - p._y, _z - p._z); }
Point Point::operator+(const Vector3& v) const { return Point(_x + v.x(), _y + v.y(), _z + v.z()); }

float Point::distanceTo(const Point& p2) const
{
	float x = p2._x - _x;
	float y = p2._y - _y;
	float z = p2._z - _z;
	return sqrt(x * x + y * y + z * z);
}

Vector3 Point::asVector3() const { return Vector3(_x, _y, _z); }
glm::vec3 Point::toGlmVec3() const { return glm::vec3(_x, _y, _z); }
glm::vec4 Point::toGlmVec4() const { return glm::vec4(_x, _y, _z, 1); }
