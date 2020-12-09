#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "vertex.h"

class Surface;

class Triangle
{
public:	
	Triangle() { }
	Triangle( const Vertex & v0, const Vertex & v1, const Vertex & v2, Surface * surface = NULL );

	Vertex getVertex( const int i );	
	Surface* surface();

private:
	Vertex _vertices[3]; /*!< Vrcholy trojúhelníka. Nic jiného tu nesmí být, jinak padne VBO v OpenGL! */	
};

#endif
