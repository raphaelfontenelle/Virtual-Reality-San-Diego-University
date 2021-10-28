#pragma once

// TODO remove dependency on GLM
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/glm.hpp>

#include "Vector3.h"

class Point
{
	public:
		Point(float x, float y, float z);
		Point(glm::vec3 p);
		Point(glm::vec4 p);
		Point();

		Vector3 operator-(const Point& p) const;
		void operator+=(const Vector3& v);
		Point operator+(const Vector3& v) const;

		float distanceTo(const Point& p2) const;

		Vector3 asVector3() const;
		glm::vec3 toGlmVec3() const;
		glm::vec4 toGlmVec4() const; // homogeneous coordinates

	private:
		float _x;
		float _y;
		float _z;
};

