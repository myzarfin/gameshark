#pragma once
#include <cstdint>
#include <d3d9.h>
#include <array>

class Color {
public:
	int a, r, g, b;
	Color( ) = default;
	Color( const int r, const int g, const int b, const int a = 255 ) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
	Color(Color col, const int a) {
		this->r = col.r;
		this->g = col.g;
		this->b = col.b;
		this->a = a;
	}
	Color(float col, const int a) {
		this->r = col;
		this->g = col;
		this->b = col;
		this->a = a;
	}

	explicit Color( const uint32_t color ) {
		this->a = ( color >> 24 ) & 0xff;
		this->r = ( color >> 16 ) & 0xff;
		this->g = ( color >> 8 ) & 0xff;
		this->b = ( color & 0xff );
	}

	static Color from_uint( const uint32_t uint ) {
		return Color( uint );
	}

	static D3DCOLOR from_color( const Color col ) {
		return D3DCOLOR_ARGB( col.a, col.r, col.g, col.b );
	}

	static Color black( const int a = 255 ) { return { 0, 0, 0, a }; }
	static Color white( const int a = 255 ) { return { 255, 255, 255, a }; }
	static Color red( const int   a = 255 ) { return { 255, 0, 0, a }; }
	static Color green( const int a = 255 ) { return { 0, 255, 0, a }; }
	static Color blue( const int  a = 255 ) { return { 0, 0, 255, a }; }

	inline float rBase() const { return r / 255.f; }
	inline float gBase() const { return g / 255.f; }
	inline float bBase() const { return b / 255.f; }
	inline float aBase() const { return a / 255.f; }

	std::array<float, 3U> Base()
	{
		std::array<float, 3U> arrBaseColor = { };
		arrBaseColor.at(0) = r / 255.f;
		arrBaseColor.at(1) = g / 255.f;
		arrBaseColor.at(2) = b / 255.f;
		return arrBaseColor;
	}

	static Color HSLToRGB(float h, float s, float l) {
		float q;

		if (l < 0.5f)
			q = l * (s + 1.f);

		else
			q = l + s - (l * s);

		float p = 2 * l - q;

		float rgb[3];
		rgb[0] = h + (1.f / 3.f);
		rgb[1] = h;
		rgb[2] = h - (1.f / 3.f);

		for (int i = 0; i < 3; ++i) {
			if (rgb[i] < 0)
				rgb[i] += 1.f;

			if (rgb[i] > 1)
				rgb[i] -= 1.f;

			if (rgb[i] < (1.f / 6.f))
				rgb[i] = p + ((q - p) * 6 * rgb[i]);
			else if (rgb[i] < 0.5f)
				rgb[i] = q;
			else if (rgb[i] < (2.f / 3.f))
				rgb[i] = p + ((q - p) * 6 * ((2.f / 3.f) - rgb[i]));
			else
				rgb[i] = p;
		}

		return {
			int(rgb[0] * 255.f),
			int(rgb[1] * 255.f),
			int(rgb[2] * 255.f)
		};
	}
};
