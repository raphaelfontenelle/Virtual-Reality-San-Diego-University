#include "Shape.h"

Shape::Shape(Color diffuse, Color specular, float shininess, Color emission, Color ambient) :
	_diffuse(diffuse), _specular(specular), _shininess(shininess), _emission(emission), _ambient(ambient) {}

Color Shape::diffuse() const { return _diffuse; }
Color Shape::specular() const {	return _specular; }
float Shape::shininess() const { return _shininess; }
Color Shape::emission() const {	return _emission; }
Color Shape::ambient() const { return _ambient; }