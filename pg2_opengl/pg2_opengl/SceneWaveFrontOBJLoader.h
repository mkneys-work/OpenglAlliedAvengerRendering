#ifndef SCENE_OBJECT_LOADER
#define SCENE_OBJECT_LOADER

#pragma once

#include<string>
#include<fstream>
#include "surface.h"
#include "MaterialsLoader.h"
#include "SceneLoader.h"

class SceneWaveFrontOBJLoader : public SceneLoader {
public:
	SceneWaveFrontOBJLoader(std::string sceneObjectsFilePath, std::string sceneWaveFrontOBJFileName, std::string texturePath);
	void LoadSceneObjects(std::vector<Surface*>& surfaces, std::vector<Material*>& materials);

private:
	void loadOBJFileLine();

	bool tryLoadMaterialsFromFile();
	bool tryLoadVertex();
	bool tryLoadNormal();
	bool tryLoadTextureCoord();
	bool tryLoadFaceVertex();
	bool tryLoadSurfaceMaterialName();
	bool tryLoadSurface();

	const static int INDEX_DIGIT_COUNT_CONST = 10;
	const static int COUNT_OF_VERTICES_PER_FACE = 3;
};

#endif // !SCENE_OBJECT_LOADER


