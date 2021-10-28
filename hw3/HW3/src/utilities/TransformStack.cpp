#include "TransformStack.h"

TransformStack::TransformStack()
{
	_transformationStack = std::stack<glm::mat4>();
	_transformationStack.push(glm::mat4()); // Start with identity
}

glm::mat4& TransformStack::top()
{
	return _transformationStack.top();
}

void TransformStack::pushTransform()
{
	_transformationStack.push(_transformationStack.top());
}

void TransformStack::popTransform()
{
	_transformationStack.pop();
}

void TransformStack::scale(float sx, float sy, float sz)
{
	glm::mat4 m = glm::mat4(); // Start with identity matrix

	m[0][0] = sx;
	m[1][1] = sy;
	m[2][2] = sz;
	// sw = 1

	// Conversion to column-major is unnecessary as matrix is diagonal
	rightmultiply(m);
}

void TransformStack::translate(float tx, float ty, float tz)
{
	glm::mat4 m = glm::mat4(); // Start with identity

	m[0][3] = tx;
	m[1][3] = ty;
	m[2][3] = tz;

	// Convert to column-major
	m = glm::transpose(m);

	rightmultiply(m);
}

void TransformStack::rotate(float angle_radians, const Vector3& axis)
{
	float cos_rads = glm::cos(angle_radians);
	// Direction is already normalised.
	glm::mat3 a_star = glm::mat3(0, axis.z(), -axis.y(), -axis.z(), 0, axis.x(), axis.y(), -axis.x(), 0);
	glm::mat3 m_3 = cos_rads * glm::mat3() + (1 - cos_rads) * axis.outerProduct(axis) + glm::sin(angle_radians) * a_star;
	glm::mat4 m(m_3);
	rightmultiply(m);
}

// From HW2 skeleton code.
void TransformStack::rightmultiply(const glm::mat4& M)
{
	glm::mat4& T = _transformationStack.top();
	T = T * M;
}
