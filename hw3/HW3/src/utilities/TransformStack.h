#pragma once
#include <stack>
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/glm.hpp>
#include "../model/Vector3.h"

class TransformStack
{
	public:
		TransformStack();

		glm::mat4& top();

		void pushTransform();
		void popTransform();

		void scale(float sx, float sy, float sz);
		void translate(float tx, float ty, float tz);
		void rotate(float angle_radians, const Vector3& axis); // Assumes axis is already normalized

	private:
		std::stack<glm::mat4> _transformationStack;
		void rightmultiply(const glm::mat4& M);
};

