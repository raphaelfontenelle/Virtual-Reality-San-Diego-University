#include "ProgressBar.hpp"  // https://github.com/prakhar1989/progress-cpp
#include "Scene.h"
#include "./shapes/Shape.h"

const Color AMBIENT_GLOBAL_DEFAULT = Color(0.0f, 0.0f, 0.0f);

Scene::Scene(int width, int height, Camera camera, std::vector<Triangle> triangles, std::vector<Sphere> spheres, 
	std::vector<DirectionalLight> directional_lights, std::vector<PointLight> point_lights) :
	_width(width), _height(height), _camera(camera), _ambient_global(AMBIENT_GLOBAL_DEFAULT),
	_triangles(triangles), _spheres(spheres), _directional_lights(directional_lights), _point_lights(point_lights)
{
	// Precompute these values now since they will be needed repeatedly and won't change.
	_aspect_ratio = (float)_width / (float)_height;
	_width_over_two = _width / 2.0f;
	_height_over_two = _height / 2.0f;
	float tan_fovy_over_two = tan(_camera.fovy_radians / 2);
	float tan_fovx_over_two = tan_fovy_over_two * _aspect_ratio;
	_alpha_multiplicand = tan_fovx_over_two / _width_over_two;
	_beta_multiplicand = tan_fovy_over_two / _height_over_two;

	// Set up basis vectors for camera space.
	Point center = _camera.lookat;	// center is what we are looking at
	Point eye = _camera.lookfrom;	// eye is the camera location
	Vector3 a = eye - center;
	Vector3 b = _camera.up;
	_w = a.normalize();
	_u = b.cross(_w);
	_u = _u.normalize();
	_v = _w.cross(_u);
}

int Scene::width() const { return _width; }
int Scene::height() const {	return _height; }

Ray Scene::rayThroughPixel(int i, int j) const
{
	// Adjustment to hit center of pixel, rather than corner.
	float i_float = i + 0.5f;
	float j_float = j + 0.5f;

	float alpha = (j_float - _width_over_two) * _alpha_multiplicand;
	float beta = (_height_over_two - i_float) * _beta_multiplicand;

	Vector3 direction = alpha * _u + beta * _v - _w;
	direction = direction.normalize();

	// The ray has origin at the camera location and looks in the given direction
	return Ray(_camera.lookfrom, direction); 
}



std::optional<Intersection> Scene::intersect(const Ray& ray) const
{
	float smallest_intersection_distance = INFINITY;
	std::optional<Intersection> closest_intersection;

	for (const Triangle& triangle : _triangles) {
		std::optional<Intersection> result = triangle.intersect(ray);
		if (result.has_value() && result.value().distance < smallest_intersection_distance) {
			smallest_intersection_distance = result.value().distance;
			closest_intersection.emplace(result.value());
		}
	}

	for (const Sphere& sphere : _spheres) {
		std::optional<Intersection> result = sphere.intersect(ray);
		if (result.has_value() && result.value().distance < smallest_intersection_distance) {
			smallest_intersection_distance = result.value().distance;
			closest_intersection.emplace(result.value());
		}
	}

	return closest_intersection;
}

Color Scene::findColor(Intersection intersection, int recursive_depth_permitted) const
{
	// Base case: no more reflections permitted
	if (recursive_depth_permitted < 1) {
		return Color(); // Colors are additive so we can just return black.
	}

	// Start with per-object ambient and emission terms
	const Shape* intersected_shape = intersection.intersected_shape;
	Color final_color = intersected_shape->ambient() + intersected_shape->emission();

	for (PointLight point_light : _point_lights) {
		final_color += point_light.computeContribution(intersection, this);
	}

	for (DirectionalLight directional_light : _directional_lights) {
		final_color += directional_light.computeContribution(intersection, this);
	}

	// Avoids recursive calls unless the specular component of the intersected object is significant.
	if (recursive_depth_permitted > 1 && intersected_shape->specular().isNonZero()) {
		Vector3 original_ray_direction = intersection.ray.direction();
		Point reflected_ray_origin = intersection.intersection_location;
		Vector3 reflected_ray_direction = original_ray_direction + 2.0f * (-original_ray_direction).dot(intersection.normal) * intersection.normal;
		reflected_ray_direction = reflected_ray_direction.normalize();

		// Push reflected ray slightly out to avoid self-intersection due to floating point inaccuracy.
		reflected_ray_origin += 0.0001f * reflected_ray_direction;

		Ray reflected_ray = Ray(reflected_ray_origin, reflected_ray_direction);
		std::optional<Intersection> reflected_intersection = intersect(reflected_ray);

		if (reflected_intersection.has_value()) {
			Color recursive_specular_component = intersected_shape->specular() * findColor(reflected_intersection.value(), recursive_depth_permitted - 1);
			final_color += recursive_specular_component;
		}
	}

	return final_color;
}

BYTE* Scene::raytrace(int max_recursion_depth) const
{
	int pix = _width * _height;
	BYTE* pixels = new BYTE[3 * pix];
	ProgressBar progressBar(_height, 70);
	RGBTRIPLE pixel_color_triple;

	for (int i = 0; i < _height; i++) {
		for (int j = 0; j < _width; j++) {
			// Finds closest object that is intersected by the ray through pixel (i, j)
			Ray ray = rayThroughPixel(i, j);
			std::optional<Intersection> intersection = intersect(ray);

			// Shades pixel (i, j)
			if (intersection.has_value()) {
				pixel_color_triple = findColor(intersection.value(), max_recursion_depth).to_freeimage_rgbtriple();
			}
			else { // No intersection so use global ambient value.
				pixel_color_triple = _ambient_global.to_freeimage_rgbtriple();
			}

			// Copies the raw color data for this pixel over the to pixels array. Each pixel is three bytes of data.
			int index_offset = (i * _width * 3) + (j * 3);
			memcpy(pixels + index_offset, &pixel_color_triple, sizeof RGBTRIPLE);

		}
		++progressBar;
		progressBar.display();
	}
	  
	progressBar.done();
	return pixels;
}
