#include "pch.h"
#include "SceneWaveFrontOBJLoader.h"
#include <iostream>
#include "utils.h"
#include "MTLMaterialFileLoader.h"
#include <iterator>

SceneWaveFrontOBJLoader::SceneWaveFrontOBJLoader(std::string sceneObjectsFilePath, std::string sceneWaveFrontOBJFileName, std::string texturePath) 
	: SceneLoader(sceneObjectsFilePath, sceneWaveFrontOBJFileName, texturePath) {
}

void SceneWaveFrontOBJLoader::LoadSceneObjects(std::vector<Surface*>& surfaces, std::vector<Material*>& materials) {
	this->materials = &materials;
	this->surfaces = &surfaces;
	const char delim[] = "\n";

	currenObjFileLine = strtok(fileBuffer, delim);

	while (currenObjFileLine != NULL)
	{
		loadOBJFileLine();
		currenObjFileLine = strtok(NULL, delim);
	}
	printf("asfa");
}

void SceneWaveFrontOBJLoader::loadOBJFileLine() {
	if (tryLoadVertex()) return;
	if (tryLoadNormal()) return;
	if (tryLoadTextureCoord()) return;
	if (tryLoadMaterialsFromFile()) return;
	if (tryLoadFaceVertex()) return;
	if (tryLoadSurfaceMaterialName()) return;
	if (tryLoadSurface()) return;
}

bool SceneWaveFrontOBJLoader::tryLoadSurface(){
	if (currenObjFileLine[0] != 'g') {
		return false;
	}

	if (faceVertices.size() > 0) {
		surfaces->push_back(new Surface(surfaceGroupName, faceVertices));
		printf("\r%I64u group(s)\t\t", surfaces->size());
		++surfaceCount;
		faceVertices.clear();

		for (int i = 0; i < static_cast<int>(materials->size()); ++i)
		{
			if (materials->at(i)->getName().compare(surfaceMaterialName) == 0)
			{
				Surface * s = *--surfaces->end();
				s->setMaterial(materials->at(i));
				break;
			}
		}
	}

	char surfaceGroupNameCharArray[128];
	sscanf(currenObjFileLine, "%*s %s", &surfaceGroupNameCharArray);
	surfaceGroupName = std::string(surfaceGroupNameCharArray);

	return true;
}


bool SceneWaveFrontOBJLoader::tryLoadMaterialsFromFile() {
	if (currenObjFileLine[0] != 'm') {
		return false;
	}
	
	std::vector<std::string> propertyTokens = splitWords(currenObjFileLine);
	MaterialFileLoader* materialLoader = new MTLMaterialFileLoader(sceneObjectsFilePath, propertyTokens[1], texturePath);
	materialLoader->loadMaterials(*materials);
	delete materialLoader;

	return true;
}

bool SceneWaveFrontOBJLoader::tryLoadVertex() {
	if (currenObjFileLine[0] != 'v' || currenObjFileLine[1] != ' ') {
		return false;
	}

	Vector3 newVertex;
	sscanf(currenObjFileLine, "%*s %f %f %f", &newVertex.x, &newVertex.y, &newVertex.z);
	verticesPosstions.push_back(newVertex);
	return true;
}

bool SceneWaveFrontOBJLoader::tryLoadNormal() {
	if (currenObjFileLine[0] != 'v' || currenObjFileLine[1] != 'n') {
		return false;
	}
	
	Vector3 newNormal;
	sscanf(currenObjFileLine, "%*s %f %f %f", &newNormal.x, &newNormal.y, &newNormal.z);
	perVertexNormals.push_back(newNormal);
	return true;
	
}

bool SceneWaveFrontOBJLoader::tryLoadTextureCoord() {
	if (currenObjFileLine[0] != 'v' || currenObjFileLine[1] != 't') {
		return false;
	}
	
	Coord2f newTexCoord;
	sscanf(currenObjFileLine, "%*s %f %f", &newTexCoord.u, &newTexCoord.v);
	textureCoords.push_back(newTexCoord);
	return true;
	
}


bool SceneWaveFrontOBJLoader::tryLoadFaceVertex() {
	if (currenObjFileLine[0] != 'f') {
		return false;
	}

	char faceVertexLines[COUNT_OF_VERTICES_PER_FACE][INDEX_DIGIT_COUNT_CONST * 3 + 2];
	sscanf(currenObjFileLine, "%*s %s %s %s", &faceVertexLines[0], &faceVertexLines[1], &faceVertexLines[2]);

	for (int i = 0; i < COUNT_OF_VERTICES_PER_FACE; i++) {
		int pointIndex, normalIndex, texCoordIndex = 0;
		Vertex vertex;

		sscanf(faceVertexLines[i], "%d/%d/%d", &pointIndex, &texCoordIndex, &normalIndex);
		vertex = Vertex(verticesPosstions[pointIndex - 1], perVertexNormals[normalIndex - 1], Vector3(0, 0, 0), &textureCoords[texCoordIndex - 1]);
		faceVertices.push_back(vertex);
	}

	return true;
}

bool SceneWaveFrontOBJLoader::tryLoadSurfaceMaterialName() {
	if (currenObjFileLine[0] != 'u') {
		return false;
	}

	char surfaceMaterialNameCharArray[128];
	sscanf(currenObjFileLine, "%*s %s", &surfaceMaterialNameCharArray);
	surfaceMaterialName = std::string(surfaceMaterialNameCharArray);
	return true;
}
