#ifndef SCENE_LOADER
#define SCENE_LOADER

#pragma once

#include<string>
#include<fstream>
#include "surface.h"
#include "MaterialFileLoader.h"

class SceneLoader {
public:
	SceneLoader(std::string sceneObjectsFilePath, std::string sceneOBJFileName, std::string texturePath);
	virtual void LoadSceneObjects(std::vector<Surface*>& surfaces, std::vector<Material*>& materials) = 0;
	~SceneLoader();
protected:
	std::string sceneObjectsFilePath;
	std::string sceneObjectsFileName;
	std::ifstream sceneObjectsFileStream;
	std::string texturePath;
	char* fileBuffer;
	char* currenObjFileLine;

	std::vector<Surface*>* surfaces;
	std::vector<Material*>* materials;

	std::vector<Vector3> verticesPosstions;
	std::vector<Vector3> perVertexNormals;
	std::vector<Coord2f> textureCoords;
	std::vector<Vertex> faceVertices;
	std::string surfaceMaterialName;
	std::string surfaceGroupName;
	int surfaceCount = 0;
};


#endif // !SCENE_OBJECT_LOADER


