#pragma once
// TODO remove dependency on GLM
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/glm.hpp>
#include <optional>

#include "../Color.h"
#include "../Ray.h"

struct Intersection;

class Shape  // abstract
{
	public:
		Color diffuse() const; 
		Color specular() const;
		float shininess() const;
		Color emission() const;
		Color ambient() const;

		virtual std::optional<Intersection> intersect(const Ray& ray) const = 0;

	protected:
		Shape(Color diffuse, Color specular, float shininess, Color emission, Color ambient);

		const Color _diffuse;
		const Color _specular;
		const float _shininess;
		const Color _emission;
		const Color _ambient;
};

// Stores data related to an intersection between a Ray and a Shape
struct Intersection {
	const Shape* intersected_shape;
	Point intersection_location;
	Vector3 normal;
	Ray ray;
	float distance;
};