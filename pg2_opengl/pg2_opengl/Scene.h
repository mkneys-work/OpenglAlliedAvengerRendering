#ifndef SCENE
#define SCENE

#pragma once

#include<string>
#include<vector>
#include "material.h"
#include "surface.h"
#include "SceneLoader.h"

class Scene {
public:
	Scene(std::string sceneStoragePath, std::string sceneStorageFileName, const std::string texturePath);
	
	void reloadSceneFromFileStorage(std::string sceneStoragePath, std::string sceneStorageFileName);

	std::vector<Surface*> getSurfaces();
	std::vector<Material*> getMaterials();

private:
	std::vector<Surface*> _surfaces;
	std::vector<Material*> _materials;
	std::string _texturePath;
	SceneLoader* _sceneLoader = nullptr;
};


#endif


