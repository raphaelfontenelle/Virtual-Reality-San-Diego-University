#include <cstdlib>

#include "Color.h"

Color::Color(float red, float green, float blue) : _red(red), _green(green), _blue(blue) {}
Color::Color(glm::vec3 rgb) : Color(rgb.r, rgb.g, rgb.b) {}
Color::Color() : Color(0.0, 0.0, 0.0) {}

RGBTRIPLE Color::to_freeimage_rgbtriple() const
{
	RGBTRIPLE triple;
	triple.rgbtBlue = color_channel_float_to_byte(_blue);
	triple.rgbtGreen = color_channel_float_to_byte(_green);
	triple.rgbtRed = color_channel_float_to_byte(_red);
	return triple;
}

void Color::operator=(const Color& c)
{
	_red = c._red;
	_green = c._green;
	_blue = c._blue;
}

void Color::operator+=(const Color& c)
{
	_red += c._red;
	_green += c._green;
	_blue += c._blue;
}

void Color::operator/=(float f)
{
	_red /= f;
	_green /= f;
	_blue /= f;
}

Color Color::operator+(const Color& c) const { return Color(_red + c._red, _green + c._green, _blue + c._blue); }
Color Color::operator*(const Color& c) const { return Color(_red * c._red, _green * c._green, _blue * c._blue); }
Color Color::operator*(float f) const {	return Color(_red * f, _green * f, _blue * f); }

bool Color::isNonZero() const
{
	return abs(_red) > 0.0001f && abs(_green) > 0.0001f && abs(_blue) > 0.0001f;
}

BYTE Color::color_channel_float_to_byte(float color_channel) const
{
	return (BYTE)floor(color_channel >= 1.0 ? 255 : color_channel * 256.0);
}
