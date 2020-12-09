#include "pch.h"
#include "SceneLoader.h"
#include <iostream>
#include "utils.h"

SceneLoader::SceneLoader(std::string sceneObjectsFilePath, std::string sceneObjectsFileName, std::string texturePath) {
	this->sceneObjectsFilePath = sceneObjectsFilePath;
	this->sceneObjectsFileName = sceneObjectsFileName;
	this->sceneObjectsFileStream.exceptions(std::ifstream::badbit);
	this->texturePath = texturePath;

	try {
		this->sceneObjectsFileStream.open(this->sceneObjectsFilePath + sceneObjectsFileName);

		size_t fileSize = static_cast<size_t>(GetFileSize64((sceneObjectsFilePath + sceneObjectsFileName).c_str()));
		this->fileBuffer = new char[fileSize + 1];
		sceneObjectsFileStream.read(fileBuffer, fileSize);
		int numberOfChars = sceneObjectsFileStream.gcount();
		fileBuffer[numberOfChars] = 0;
	}
	catch (std::system_error& e) {
		std::cerr << e.code().message() << std::endl;
	}
}

SceneLoader::~SceneLoader() {
	try {
		this->sceneObjectsFileStream.close();
		delete[] fileBuffer;
	}
	catch (std::system_error& e) {
		std::cerr << e.code().message() << std::endl;
	}
}

