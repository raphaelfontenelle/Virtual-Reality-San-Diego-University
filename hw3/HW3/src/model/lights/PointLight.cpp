#include <algorithm> // max
#include <optional>

#include "PointLight.h"
#include "../Ray.h"
#include "../Scene.h"

PointLight::PointLight(Color color, Attenuation attenuation, Point point) : Light(color), _point(point), _attenuation(attenuation) {}

Vector3 PointLight::directionFrom(const Point& point) const
{
	Vector3 light_direction = _point - point;
	return light_direction.normalize();
}

Color PointLight::computeContribution(Intersection hit_object, const Scene* scene) const
{
	Vector3 shadow_ray_direction = directionFrom(hit_object.intersection_location);

	// Bring origin of ray slightly towards light source to prevent self-intersection.
	Point shadow_ray_origin = hit_object.intersection_location + shadow_ray_direction * 0.001f;

	Ray shadow_ray = Ray(shadow_ray_origin, shadow_ray_direction);
	std::optional<Intersection> shadow_ray_intersection = scene->intersect(shadow_ray);
	float distance_ho_to_light = hit_object.intersection_location.distanceTo(_point);

	// The distance to the light is smaller than the distance to the first object intersection, so the light is visible
	if (!shadow_ray_intersection.has_value() || distance_ho_to_light < shadow_ray_intersection.value().distance) {
		// diffuse
		float l_dot_n = shadow_ray_direction.dot(hit_object.normal);
		Color light_contribution = _color * hit_object.intersected_shape->diffuse() * std::max(l_dot_n, 0.0f);

		// specular
		Vector3 half_angle = (-hit_object.ray.direction()) + shadow_ray_direction;
		half_angle = half_angle.normalize();
		float h_dot_n = half_angle.dot(hit_object.normal);

		light_contribution += _color * hit_object.intersected_shape->specular() * (pow(std::max(h_dot_n, 0.0f), hit_object.intersected_shape->shininess()));

		// Handle attenuation for Point Lights
		float attenuation_denominator = 
			_attenuation.constant + 
			(_attenuation.linear * distance_ho_to_light) + 
			(_attenuation.quadratic * distance_ho_to_light * distance_ho_to_light);
		light_contribution /= attenuation_denominator;

		return light_contribution;
	}
	else {
		return Color();
	}
}
