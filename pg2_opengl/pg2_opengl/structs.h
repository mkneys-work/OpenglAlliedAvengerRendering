#ifndef STRUCTS_H
#define STRUCTS_H

#pragma once

#include "vector3.h"

struct Vertex3f
{
	float x, y, z;

	operator Vector3() const;
};

struct Normal3f : public Vertex3f
{
	Normal3f( const float x, const float y, const float z )
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	void unify( const float v_x, const float v_y, const float v_z )
	{
		if ( ( x * v_x + y * v_y + z * v_z ) > 0.0f )
		{
			x *= -1.0f;
			y *= -1.0f;
			z *= -1.0f;
		}

		Vector3 n( x, y, z );
		n.Normalize();
		x = n.x;
		y = n.y;
		z = n.z;
	}

	Normal3f operator* ( const float a ) const;
};

struct Coord2f { float u, v; };

float cLinear(const float cSrgb, const float gamma = 2.4f);
float cSrgb(const float cLinear, const float gamma = 2.4f);

struct Color4f
{
	float r, g, b;
	float a;
};

struct Color3f
{
	float r, g, b;

	Color3f(const float r = 0.0f, const float g = 0.0f, const float b = 0.0f) : r(r), g(g), b(b) { }

	operator Color4f() const;

	Color3f operator* (const float x) const;

	Color3f linear(const float gamma = 2.4f) const;
	Color3f srgb(const float gamma = 2.4f) const;

	template<typename T> static Color3f makeFromBgr(BYTE * p)
	{
		return Color3f{ float(((T*)(p))[2]), float(((T*)(p))[1]), float(((T*)(p))[0]) };
	}
};

Color3f operator+ (const Color3f & x, const Color3f & y);
Color3f operator* (const Color3f & x, const Color3f & y);

#endif