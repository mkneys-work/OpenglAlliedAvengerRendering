#ifndef MATERIAL_LOADER
#define MATERIAL_LOADER

#pragma once

#include<string>
#include <fstream>
#include <vector>
#include "MaterialFileLoader.h"

class MaterialsLoader {
public:
	MaterialsLoader(const std::string materialFilesPath, const std::string texturePath);
	MaterialsLoader(const std::string materialFilesPath, std::map<std::string, MaterialFileType> materialsFileTypeNamesMap, const std::string texturePath);
	void loadMaterialsFromFiles(std::vector<Material*>& materials, bool eraseLoadersFiles = true);

	void addMaterialFileToLoader(std::string materialFileName, MaterialFileType materialFileType);
	~MaterialsLoader();
private:
	std::string _materialFilesPath;
	std::string _texturePath;
	std::vector<MaterialFileLoader*> _materialFileLoaders;

	MaterialFileLoader* createMaterialFileLoader(MaterialFileType materialFileType, std::string materialFileName);
	void deleteFileLoaders();
};

#endif