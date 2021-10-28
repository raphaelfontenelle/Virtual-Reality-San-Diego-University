#include "Triangle.h"

// Vertexes and normal are stored in world-space (after transform) not object-space
Triangle::Triangle(Color diffuse, Color specular, float shininess, Color emission, Color ambient, glm::mat4 transform, Point a, Point b, Point c) :
	Shape(diffuse, specular, shininess, emission, ambient),
	_a(Point(transform * a.toGlmVec4())),
	_b(Point(transform * b.toGlmVec4())),
	_c(Point(transform * c.toGlmVec4()))
{
	_normal = (_b - _a).cross(_c - _a);
	_normal = _normal.normalize();

	// Pre-computed values for barycentric coordinates
	_v0 = _b - _a;
	_v1 = _c - _a;
	_d00 = _v0.dot(_v0);
	_d01 = _v0.dot(_v1);
	_d11 = _v1.dot(_v1);
	_invDenom = 1.0 / (_d00 * _d11 - _d01 * _d01);
}


std::optional<Intersection> Triangle::intersect(const Ray& ray) const
{
	// Triangles are stored with the transform already applied to the vertexes, so we do not need to worry about transforms here.
	// Begin ray-plane intersection
	Point p0 = ray.origin();
	Vector3 p1 = ray.direction();
	
	float denominator = p1.dot(_normal);

	// No intersection when parallel to the plane, with a small tolerance here for floating point error
	if (abs(denominator) < 0.0001f) {
		return std::nullopt;
	}

	float t = (_a.asVector3().dot(_normal) - p0.asVector3().dot(_normal)) / denominator;
	
	// The ray intersects the plane in the negative direction.
	if (t <= 0) {
		return std::nullopt;
	}

	// Begin compute barycentric coordinates.
		// From https://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates
	Point p = p0 + t * p1;
	Vector3 v2 = p - _a;

	float d20 = v2.dot(_v0);
	float d21 = v2.dot(_v1);
	float beta = (_d11 * d20 - _d01 * d21) * _invDenom;
	float gamma = (_d00 * d21 - _d01 * d20) * _invDenom;
	float alpha = 1.0f - beta - gamma;
	// End compute barycentric coordinates.

	if (alpha >= 0 && alpha <= 1 && beta >= 0 && beta <= 1 && gamma >= 0 && gamma <= 1) {
		return { { this, p, _normal, ray, t } };
	}
	else {
		return std::nullopt;
	}
}