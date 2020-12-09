#include "pch.h"
#include "surface.h"

Surface::Surface()
{
	this->_trianglesCount = 0;
	this->_triangles = NULL;
}

Surface::Surface( const std::string & name, std::vector<Vertex> & faceVertices)
{
	const int verticesCount = static_cast<int>(faceVertices.size());
	assert((verticesCount > 0) && (verticesCount % 3 == 0));
	_name = name;
	_trianglesCount = verticesCount / 3;
	_triangles = new Triangle[_trianglesCount];

	for (int i = 0; i < _trianglesCount; ++i)
	{
		getTriangles()[i] = Triangle(faceVertices[i * 3], faceVertices[i * 3 + 1], faceVertices[i * 3 + 2], this);
	}
}

Surface::~Surface()
{
	if ( this->_triangles )
	{
		delete[] _triangles;
		_triangles = nullptr;
	}
	_trianglesCount = 0;
}

Triangle & Surface::getTriangle( const int i )
{
	return _triangles[i];
}

Triangle * Surface::getTriangles()
{
	return _triangles;
}

std::string Surface::getName()
{
	return _name;
}

int Surface::getTrianglesCount()
{
	return _trianglesCount;
}

int Surface::getVerticesCount()
{
	return 3 * _trianglesCount;
}

void Surface::setMaterial( Material * material )
{
	_material = material;
}

Material * Surface::getMaterial() const
{
	return _material;
}
