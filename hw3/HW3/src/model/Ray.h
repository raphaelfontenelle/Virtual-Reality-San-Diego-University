#pragma once
#include "Point.h"
#include "Vector3.h"

class Ray
{
	public:
		Ray(Point origin, Vector3 direction);
		Ray(glm::vec3 origin, glm::vec3 direction);
		Ray();
		
		Point origin() const;
		Vector3 direction() const;

		Ray toObjectCoordinates(const glm::mat4& m_inverse) const;

	private:
		const Point _origin;
		const Vector3 _direction;
};

