#ifndef SURFACE_H_
#define SURFACE_H_

#include "vertex.h"
#include "material.h"
#include "triangle.h"

class Surface
{
public:
	Surface();
	Surface(const std::string & name, std::vector<Vertex> & face_vertices);

	~Surface();	

	Triangle & getTriangle( const int i );
	Triangle * getTriangles();
	std::string getName();
	int getTrianglesCount();
	int getVerticesCount();	
	void setMaterial( Material * material );
	Material * getMaterial() const;

private:
	int _trianglesCount{ 0 };
	Triangle * _triangles{ nullptr };

	std::string _name{ "unknown" };
	Material * _material{ nullptr };
};

#endif
