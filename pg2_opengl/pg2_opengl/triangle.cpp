#include "pch.h"
#include "triangle.h"

Triangle::Triangle( const Vertex & v0, const Vertex & v1, const Vertex & v2, Surface * surface )
{
	_vertices[0] = v0;
	_vertices[1] = v1;
	_vertices[2] = v2;

	*reinterpret_cast<Surface **>( &_vertices[0].pad ) = surface;	
}

Vertex Triangle::getVertex( const int i )
{
	return _vertices[i];
}

Surface * Triangle::surface()
{	
	return *reinterpret_cast<Surface **>( _vertices[0].pad );
}
