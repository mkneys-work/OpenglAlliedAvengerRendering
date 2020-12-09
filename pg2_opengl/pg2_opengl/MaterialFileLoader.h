#ifndef MATERIAL_FILE_LOADER
#define MATERIAL_FILE_LOADER

#pragma once

#include<string>
#include <fstream>
#include <vector>
#include "material.h"

enum class MaterialFileType : char {MTL = 1};

class MaterialFileLoader 
{
public:	
	MaterialFileLoader(const std::string materialFilePath, const std::string materialFileName, const std::string texturesPath);

	virtual void loadMaterials(std::vector<Material*>& materials) = 0;

	~MaterialFileLoader();

protected:
	std::string _texturesPath;
	std::ifstream _materialFileInput;
};

#endif