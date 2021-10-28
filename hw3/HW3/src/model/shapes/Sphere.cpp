#include "Sphere.h"

Sphere::Sphere(Color diffuse, Color specular, float shininess, Color emission, Color ambient, glm::mat4 transform, Point center, float radius) :
	Shape(diffuse, specular, shininess, emission, ambient), _center(center), _radius_squared(radius * radius),
	_transform(transform),
	_transform_inverse(glm::inverse(_transform)) {}

std::optional<Intersection> Sphere::intersect(const Ray& ray) const 
{
	// Transform ray to object coordinates.
	// Do sphere-ray' intersection to get p'
	Ray ray_prime = ray.toObjectCoordinates(_transform_inverse);

	Point p0_prime = ray_prime.origin();
	Vector3 p1_prime = ray_prime.direction();
	Point center = _center;
	Vector3 p0_prime_minus_c = p0_prime - center;

	// Set up quadratic equation with coefficients a, b, c
	float a = p1_prime.dot(p1_prime);
	float b = (2.0f *  p1_prime).dot(p0_prime_minus_c);
	float c = p0_prime_minus_c.dot(p0_prime_minus_c) - _radius_squared;

	float discriminant = b * b - (4.0f * a * c);

	// Only complex roots
	if (discriminant < 0) {
		return std::nullopt;
	}

	float discriminant_sqrt = sqrt(discriminant);
	float denominator = (2.0f * a);

	float t_root_one = (-b + discriminant_sqrt) / denominator;
	float t_root_two = (-b - discriminant_sqrt) / denominator;

	float smallest_positive_root;

	if (t_root_one == t_root_two) {
		// Treat the case where the ray is exactly tangent to the sphere as no intersection.
		return std::nullopt;
	}

	// Two positive roots, choose smallest
	if (t_root_one > 0 && t_root_two > 0) {
		smallest_positive_root = glm::min(t_root_one, t_root_two);
	}
	else if (t_root_one > 0) {
		smallest_positive_root = t_root_one;
	}
	else if (t_root_two > 0) {
		smallest_positive_root = t_root_two;
	}
	else {
		// Shouldn't reach this case since we checked the discriminant
		return std::nullopt;
	}

	// TODO remove dependency on glm
	glm::vec4 p_prime = glm::vec4((p0_prime + smallest_positive_root * p1_prime).toGlmVec3(), 1);
	glm::vec4 normal_prime = p_prime - glm::vec4(center.toGlmVec3(), 1);
	glm::vec4 normal4 = glm::transpose(_transform_inverse) * normal_prime;
	Vector3 normal = Vector3(normal4.x, normal4.y, normal4.z);
	normal = normal.normalize();
	glm::vec4 p = _transform * p_prime;

	Point p3 = Point(p.x / p.w, p.y / p.w, p.z / p.w);

	return { { this, p3, normal, ray, ray.origin().distanceTo(p3) } };
}