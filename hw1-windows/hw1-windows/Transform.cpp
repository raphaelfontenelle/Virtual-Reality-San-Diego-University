// Transform.cpp: implementation of the Transform class.

#include "Transform.h"

//Please implement the following functions:

// Helper rotation function.  
mat3 Transform::rotate(const float degrees, const vec3& axis) {

	float d = glm::radians(degrees);
	glm::mat3 m(cos(d) + axis.x * axis.x * (1 - cos(d)), axis.x * axis.y * (1 - cos(d)) - axis.z * sin(d), axis.x * axis.z * (1 - cos(d)) + axis.y * sin(d),
				axis.y * axis.x * (1 - cos(d)) + axis.z*sin(d), cos(d) + axis.y * axis.y * (1 - cos(d)), axis.y * axis.z * (1 - cos(d)) - axis.x * sin(d),
				axis.z * axis.x * (1 - cos(d)) - axis.y * sin(d), axis.z * axis.y*(1 - cos(d)) + axis.x * sin(d), cos(d) + axis.z * axis.z * (1 - cos(d)));
	m = glm::transpose(m);
	
  return m;
}

// Transforms the camera left around the "crystal ball" interface
void Transform::left(float degrees, vec3& eye, vec3& up) {
  // YOUR CODE FOR HW1 HERE
	const vec3 rotationAxis = glm::normalize(up);
	eye = Transform::rotate(degrees, rotationAxis) * eye;
}

// Transforms the camera up around the "crystal ball" interface
void Transform::up(float degrees, vec3& eye, vec3& up) {
  // YOUR CODE FOR HW1 HERE 
	const vec3 rotationAxis = glm::normalize(glm::cross(eye, up));
	eye = Transform::rotate(degrees, rotationAxis) * eye;
	up = glm::normalize(Transform::rotate(degrees, rotationAxis) * up);
}

// Your implementation of the glm::lookAt matrix
mat4 Transform::lookAt(vec3 eye, vec3 up) {
  // YOUR CODE FOR HW1 HERE
	glm::mat4 view = glm::lookAt(
		glm::vec3(eye.x, eye.y, eye.z),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(up.x, up.y, up.z)
	);
	
  // You will change this return call
	return view;
}

Transform::Transform()
{

}

Transform::~Transform()
{

}