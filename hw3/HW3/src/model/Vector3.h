#pragma once

// TODO remove dependency on GLM
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/glm.hpp>

class Vector3
{
	public:
		Vector3(float x, float y, float z);
		Vector3();

		float x() const;
		float y() const;
		float z() const;

		void operator=(const Vector3& v);
		void operator+=(const Vector3& v2);
		Vector3 operator*(float f) const;
		Vector3 operator+(const Vector3& v2) const;
		Vector3 operator-() const;
		Vector3 operator-(const Vector3& v2) const;

		float dot(const Vector3& v2) const;
		Vector3 cross(const Vector3& v2) const;
		Vector3 normalize() const;
		glm::mat3 outerProduct(const Vector3& v2) const;

	protected:
		float _x;
		float _y;
		float _z;
};

Vector3 operator*(float f, const Vector3& v);