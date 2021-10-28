#include <algorithm> // max
#include <optional>

#include "DirectionalLight.h"
#include "../Ray.h"
#include "../Scene.h"

DirectionalLight::DirectionalLight(Color color, Vector3 direction) : 
	Light(color), _direction(direction.normalize()) {}

Color DirectionalLight::computeContribution(Intersection hit_object, const Scene* scene) const
{
	// Bring origin of ray slightly towards light source to prevent self-intersection.
	Point shadow_ray_origin = hit_object.intersection_location + _direction * 0.001f;

	Ray shadow_ray = Ray(shadow_ray_origin, _direction);
	std::optional<Intersection> shadow_ray_intersection = scene->intersect(shadow_ray);

	if (!shadow_ray_intersection.has_value()) {
		// diffuse
		float l_dot_n = _direction.dot(hit_object.normal);
		Color light_contribution = _color * hit_object.intersected_shape->diffuse() * std::max(l_dot_n, 0.0f);

		// specular
		Vector3 half_angle = (-hit_object.ray.direction()) + _direction;
		half_angle = half_angle.normalize();
		float h_dot_n = half_angle.dot(hit_object.normal);

		light_contribution += _color * hit_object.intersected_shape->specular() * (pow(std::max(h_dot_n, 0.0f), hit_object.intersected_shape->shininess()));

		return light_contribution;
	}
	else {
		return Color();
	}
}
