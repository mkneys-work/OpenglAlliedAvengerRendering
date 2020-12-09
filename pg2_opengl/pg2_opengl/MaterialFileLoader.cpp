#include "pch.h"
#include "MaterialFileLoader.h"
#include <iostream>

MaterialFileLoader::MaterialFileLoader(const std::string materialFilePath, const std::string materialFileName, const std::string texturesPath)
{	
	_texturesPath = texturesPath;
	_materialFileInput.exceptions(std::ifstream::badbit);
	
	try {
		_materialFileInput.open(materialFilePath + "/" + materialFileName);
	}
	catch (std::system_error& e) {
		std::cerr << e.code().message() << std::endl;
	}
};

MaterialFileLoader::~MaterialFileLoader()
{
	try {
		_materialFileInput.close();
	}
	catch (std::system_error& e) {
		std::cerr << e.code().message() << std::endl;
	}
}
