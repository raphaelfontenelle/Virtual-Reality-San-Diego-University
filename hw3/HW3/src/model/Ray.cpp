#include "Ray.h"

Ray::Ray(Point origin, Vector3 direction) : _origin(origin), _direction(direction.normalize()) {}

Ray::Ray(glm::vec3 origin, glm::vec3 direction) :
	Ray(Point(origin.x, origin.y, origin.z), Vector3(direction.x, direction.y, direction.z)) {}

Ray::Ray() : Ray(Point(), Vector3()) {}

Point Ray::origin() const {	return _origin; }
Vector3 Ray::direction() const { return _direction; }

Ray Ray::toObjectCoordinates(const glm::mat4& m_inverse) const
{
	// TODO remove dependency on glm
	Point origin_prime = Point(m_inverse * _origin.toGlmVec4());
	glm::vec4 direction_prime_glm = m_inverse * glm::vec4(_direction.x(), _direction.y(), _direction.z(), 0);
	Vector3 direction_prime = Vector3(direction_prime_glm.x, direction_prime_glm.y, direction_prime_glm.z);
	return Ray(origin_prime, direction_prime);
}

