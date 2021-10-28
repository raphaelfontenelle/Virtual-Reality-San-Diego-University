#pragma once
#include "../Color.h"
#include "../Point.h"
#include "../shapes/Shape.h"

class Scene;

struct Attenuation {
	float constant;
	float linear;
	float quadratic;
};

class Light  // abstract
{
	public:
		Light(Color color);

		virtual Color computeContribution(Intersection hit_object, const Scene* scene) const = 0;

	protected:
		const Color _color;
};

