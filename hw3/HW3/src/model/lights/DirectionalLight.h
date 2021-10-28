#pragma once
#include "Light.h"
#include "../Vector3.h"


class DirectionalLight :
	public Light
{
	public:
		DirectionalLight(Color color, Vector3 direction);

		Color computeContribution(Intersection intersection, const Scene* scene) const;

	private:
		const Vector3 _direction;
};

