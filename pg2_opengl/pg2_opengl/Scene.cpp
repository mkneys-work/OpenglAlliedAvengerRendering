#include "pch.h"
#include "Scene.h"
#include "SceneWaveFrontOBJLoader.h"

Scene::Scene(std::string sceneStoragePath, std::string sceneStorageFileName, const std::string texturePath) {
	_texturePath = texturePath;
	reloadSceneFromFileStorage(sceneStoragePath, sceneStorageFileName);
}

void Scene::reloadSceneFromFileStorage(std::string sceneStoragePath, std::string sceneStorageFileName) {
	_materials.clear();
	_surfaces.clear();

	if (_sceneLoader != nullptr) {
		delete _sceneLoader;
	}

	_sceneLoader = new SceneWaveFrontOBJLoader(sceneStoragePath, sceneStorageFileName, _texturePath);
	_sceneLoader->LoadSceneObjects(_surfaces, _materials);

	/*MaterialsLoader* materialsLoader = new MaterialsLoader(sceneStoragePath, materialsFileTypeNamesMap, texturePath);
	materialsLoader->loadMaterialsFromFiles(materials);

	delete materialsLoader;*/
	delete _sceneLoader;
	//materialsLoader = nullptr;
	_sceneLoader = nullptr;
}

std::vector<Surface*> Scene::getSurfaces() {
	return _surfaces;
}

std::vector<Material*> Scene::getMaterials() {
	return _materials;
}