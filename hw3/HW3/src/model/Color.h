#pragma once

// TODO remove dependency on GLM
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <FreeImage.h>
#include <glm/glm.hpp>

class Color
{
	public:
		Color(); // Initialise Color with (0, 0, 0)
		Color(float red, float green, float blue);
		Color(glm::vec3 rgb);

		RGBTRIPLE to_freeimage_rgbtriple() const;

		Color operator+(const Color& c) const;
		Color operator*(const Color& c) const;
		Color operator*(float f) const;
		void operator=(const Color& c);
		void operator+=(const Color& c);
		void operator/=(float f);

		bool isNonZero() const;

	private:
		float _red;
		float _green;
		float _blue;

		BYTE color_channel_float_to_byte(float color_channel) const;
};

