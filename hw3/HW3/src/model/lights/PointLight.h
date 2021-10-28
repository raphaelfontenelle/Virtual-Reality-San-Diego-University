#pragma once
#include "Light.h"
#include "../Point.h"

class PointLight :
	public Light
{
	public:
		PointLight(Color color, Attenuation attenuation, Point point);

		Color computeContribution(Intersection hit_object, const Scene* scene) const;

	private: 
		Vector3 directionFrom(const Point& point) const;
		const Point _point;
		const Attenuation _attenuation;
};

