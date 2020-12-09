#include "pch.h"
#include "structs.h"
#include "mymath.h"

Color3f operator+( const Color3f & x, const Color3f & y )
{
	return Color3f{ x.r + y.r, x.g + y.g, x.b + y.b };
}

Color3f operator*( const Color3f & x, const Color3f & y )
{
	return Color3f{ x.r * y.r, x.g * y.g , x.b * y.b };
}

Color3f::operator Color4f() const
{
	return Color4f{ r, g, b, 1.0f };
}

Color3f Color3f::operator*( const float x ) const
{
	return Color3f{ r * x, g * x, b * x };
}

Color3f Color3f::linear( const float gamma ) const
{	
	return Color3f{ cLinear( r ), cLinear( g ), cLinear( b ) };
}

Color3f Color3f::srgb( const float gamma ) const
{
	return Color3f{ cSrgb( r ), cSrgb( g ), cSrgb( b ) };
}

Vertex3f::operator Vector3() const
{
	return Vector3( x, y, z );
}

Normal3f Normal3f::operator*( const float a ) const
{
	return Normal3f{ x * a, y * a, z * a };
}

float cLinear(const float cSrgb, const float gamma)
{
	if (cSrgb <= 0.0f) return 0.0f;
	else if (cSrgb >= 1.0f) return 1.0f;

	assert((cSrgb >= 0.0f) && (cSrgb <= 1.0f));

	if (cSrgb <= 0.04045f)
	{
		return cSrgb / 12.92f;
	}
	else
	{
		const float a = 0.055f;
		return powf((cSrgb + a) / (1.0f + a), gamma);
	}
}

float cSrgb(const float cLinear, const float gamma)
{
	if (cLinear <= 0.0f) return 0.0f;
	else if (cLinear >= 1.0f) return 1.0f;

	assert((cLinear >= 0.0f) && (cLinear <= 1.0f));

	if (cLinear <= 0.0031308f)
	{
		return 12.92f * cLinear;
	}
	else
	{
		const float a = 0.055f;
		return (1.0f + a) * powf(cLinear, 1.0f / gamma) - a;
	}
}

