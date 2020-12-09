#include "pch.h"
#include "MaterialsLoader.h"
#include "MTLMaterialFileLoader.h"

MaterialsLoader::MaterialsLoader(const std::string materialFilesPath, std::map<std::string, MaterialFileType> materialsFileTypeNamesMap, const std::string texturePath)
	: MaterialsLoader(materialFilesPath, texturePath)
{
	for (std::pair<std::string, MaterialFileType> materialFileTypeName : materialsFileTypeNamesMap) {
		_materialFileLoaders.push_back(createMaterialFileLoader(materialFileTypeName.second, materialFileTypeName.first));
	}
}

MaterialsLoader::MaterialsLoader(const std::string materialFilesPath, const std::string texturePath) {
	_materialFilesPath = materialFilesPath;
	_texturePath = texturePath;
}

MaterialFileLoader* MaterialsLoader::createMaterialFileLoader(MaterialFileType materialFileType, std::string materialFileName) {
	switch (materialFileType) {
		case MaterialFileType::MTL:
			return new MTLMaterialFileLoader(_materialFilesPath, materialFileName, _texturePath);
			break;
		default: return nullptr;
	}
}

void MaterialsLoader::loadMaterialsFromFiles(std::vector<Material*>& materials, bool eraseLoadersFiles) {
	for (MaterialFileLoader* materialLoader : _materialFileLoaders) {
		materialLoader->loadMaterials(materials);
	}
	
	if (eraseLoadersFiles) {
		deleteFileLoaders();
	}
}

void MaterialsLoader::deleteFileLoaders() {
	for (MaterialFileLoader* materialLoader : _materialFileLoaders) {
		if (materialLoader != nullptr) {
			delete materialLoader;
			materialLoader = nullptr;
		}
	}

	_materialFileLoaders.clear();
}

void MaterialsLoader::addMaterialFileToLoader(std::string materialFileName, MaterialFileType materialFileType) {
	_materialFileLoaders.push_back(createMaterialFileLoader(materialFileType, materialFileName));
}

MaterialsLoader::~MaterialsLoader()
{
	deleteFileLoaders();
}


