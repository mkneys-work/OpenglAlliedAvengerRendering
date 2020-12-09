#ifndef VERTEX_H_
#define VERTEX_H_

#include "structs.h"
#include "vector3.h"

#define NO_TEXTURE_COORDS 1


struct Vertex
{
public:
	Vector3 position;
	Vector3 normal;
	Vector3 color;
	Coord2f texture_coords[NO_TEXTURE_COORDS];
	Vector3 tangent;
	int materialIndex;
	char pad[8];

	Vertex() { }
	Vertex( const Vector3 position, const Vector3 normal, Vector3 color, Coord2f * texture_coords = NULL );
};

#endif
