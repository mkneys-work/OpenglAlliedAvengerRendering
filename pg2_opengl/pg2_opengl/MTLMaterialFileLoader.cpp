#include "pch.h"
#include "MTLMaterialFileLoader.h"
#include <iostream>
#include "utils.h"

MTLMaterialFileLoader::MTLMaterialFileLoader(const std::string materialFilePath, const std::string materialFileName, const std::string texturesPath)
	: MaterialFileLoader(materialFilePath, materialFileName, texturesPath)
{
}

void MTLMaterialFileLoader::loadMaterials(std::vector<Material*>& materials) {
	std::string currentMtlFileLine;

	while (std::getline(_materialFileInput, currentMtlFileLine)) {
		trim(currentMtlFileLine);

		if (currentMtlFileLine.find(MTL_FILE_COMMENT_SIGN) != std::string::npos || currentMtlFileLine == "") {
			continue;
		}

		if (currentMtlFileLine.find(MTL_NEW_MATERIAL_SIGN) != std::string::npos) {
			materials.push_back(readNewMaterial(currentMtlFileLine));
			materials.at(materials.size() - 1)->setMaterialIndex(materials.size() - 1);
		}
	}
}

Material* MTLMaterialFileLoader::readNewMaterial(std::string currentFileLine) {
	Material* newMaterial = new Material();
	_currentMaterial = newMaterial;

	do {
		trim(currentFileLine);
		trySetMaterialConstants(currentFileLine);
		trySetMaterialTextures(currentFileLine);
	} while (std::getline(_materialFileInput, currentFileLine) && currentFileLine != "");
	return newMaterial;
}

void MTLMaterialFileLoader::trySetMaterialConstants(std::string currentFileLine) {
	trySetStringMtlMaterialProperty(currentFileLine, MTL_NEW_MATERIAL_SIGN,
		[](Material* material, std::string propertyValue) {return material->setName(propertyValue); });

	trySetIntMtlMaterialProperty(currentFileLine, SHADER_TYPE_SIGN,
		[](Material* material, int propertyValue) {return material->setShader(Shader(propertyValue)); });

	trySetFloatMtlMaterialProperty(currentFileLine, IOR_SIGN,
		[](Material* material, float propertyValue) {return material->setIor(propertyValue); });

	trySetFloatMtlMaterialProperty(currentFileLine, ROUGNESS_SIGN,
		[](Material* material, float propertyValue) {return material->setRoughness(propertyValue); });

	trySetFloatMtlMaterialProperty(currentFileLine, METALLICNESS_SIGN,
		[](Material* material, float propertyValue) {return material->setMetalicness(propertyValue); });

	trySetFloatMtlMaterialProperty(
		currentFileLine, 
		SHININESS_SIGN,
		[](Material* material, float propertyValue) {return material->setShininess(propertyValue); }
	);

	trySetColorMtlMaterialProperty(currentFileLine, AMBIENT_COLOR_SIGN,
		[](Material* material, Color3f propertyValue) {return material->setAmbientColor(propertyValue); });

	trySetColorMtlMaterialProperty(currentFileLine, DIFFUSE_COLOR_SIGN,
		[](Material* material, Color3f propertyValue) {return material->setDiffuseColor(propertyValue); });

	trySetColorMtlMaterialProperty(currentFileLine, EMISSION_COLOR_SIGN,
		[](Material* material, Color3f propertyValue) {return material->setEmissionColor(propertyValue); });

	trySetColorMtlMaterialProperty(currentFileLine, SPECULAR_COLOR_SIGN,
		[](Material* material, Color3f propertyValue) {return material->setSpecularColor(propertyValue); });
}

void MTLMaterialFileLoader::trySetMaterialTextures(std::string currentFileLine) {
	trySetTextureMtlMaterialProperty(currentFileLine, DIFFUSE_COLOR_MAP_SIGN, Material::TextureMapSlotNumber::DIFFUSE,
		[](Material* material, Texture* texture, Material::TextureMapSlotNumber slotNumber) {return material->setTexture(slotNumber, texture);});

	trySetTextureMtlMaterialProperty(currentFileLine, SPECULAR_COLOR_MAP_SIGN, Material::TextureMapSlotNumber::SPECULAR,
		[](Material* material, Texture* texture, Material::TextureMapSlotNumber slotNumber) {return material->setTexture(slotNumber, texture); });

	trySetTextureMtlMaterialProperty(currentFileLine, NORMAL_MAP_SIGN, Material::TextureMapSlotNumber::NORMAL,
		[](Material* material, Texture* texture, Material::TextureMapSlotNumber slotNumber) {return material->setTexture(slotNumber, texture); });

	trySetTextureMtlMaterialProperty(currentFileLine, OPACITY_MAP_SIGN, Material::TextureMapSlotNumber::OPACITY,
		[](Material* material, Texture* texture, Material::TextureMapSlotNumber slotNumber) {return material->setTexture(slotNumber, texture); });

	trySetTextureMtlMaterialProperty(currentFileLine, ROUGNESS_MAP_SIGN, Material::TextureMapSlotNumber::ROUGNESS,
		[](Material* material, Texture* texture, Material::TextureMapSlotNumber slotNumber) {return material->setTexture(slotNumber, texture); });

	trySetTextureMtlMaterialProperty(currentFileLine, METALICNESS_MAP_SIGN, Material::TextureMapSlotNumber::METALLICNESS,
		[](Material* material, Texture* texture, Material::TextureMapSlotNumber slotNumber) {return material->setTexture(slotNumber, texture); });
}

void MTLMaterialFileLoader::trySetColorMtlMaterialProperty(std::string mtlPropertyLine, std::string propertyName, void setterFunction(Material*, Color3f)) {
	std::vector<std::string> propertyTokens = splitWords(mtlPropertyLine);
	if (propertyTokens[0] != propertyName) return;
	
	float r, g, b;
	r = std::stof(propertyTokens.at(1));
	g = std::stof(propertyTokens.at(2));
	b = std::stof(propertyTokens.at(3));

	Color3f propertyValues = Color3f(r, g, b);
	setterFunction(_currentMaterial, propertyValues);
}

void MTLMaterialFileLoader::trySetFloatMtlMaterialProperty(std::string mtlPropertyLine, std::string propertyName, void setterFunction(Material*, float)) {
	std::vector<std::string> propertyTokens = splitWords(mtlPropertyLine);
	if (propertyTokens[0] != propertyName) return;

	float propertyValue = std::stof(propertyTokens.at(1));
	setterFunction(_currentMaterial, propertyValue);
}

void MTLMaterialFileLoader::trySetIntMtlMaterialProperty(std::string mtlPropertyLine, std::string propertyName, void setterFunction(Material*, int)) {
	std::vector<std::string> propertyTokens = splitWords(mtlPropertyLine);
	if (propertyTokens[0] != propertyName) return;

	int propertyValue = std::stoi(propertyTokens.at(1));;
	setterFunction(_currentMaterial, propertyValue);
}

void MTLMaterialFileLoader::trySetStringMtlMaterialProperty(std::string mtlPropertyLine, std::string propertyName, void setterFunction(Material*, std::string)) {
	std::vector<std::string> propertyTokens = splitWords(mtlPropertyLine);
	if (propertyTokens[0] != propertyName) return;
	
	setterFunction(_currentMaterial, propertyTokens.at(1));
}

void MTLMaterialFileLoader::trySetTextureMtlMaterialProperty(std::string mtlPropertyLine, std::string propertyName, Material::TextureMapSlotNumber slotNumber, void setterFunction(Material*, Texture*, Material::TextureMapSlotNumber)) {
	std::vector<std::string> propertyTokens = splitWords(mtlPropertyLine);
	if (propertyTokens[0] != propertyName) return;

	std::string fullName = _texturesPath + propertyTokens[1];
	setterFunction(_currentMaterial, loadTextureFromFile(fullName), slotNumber);
}

Texture* MTLMaterialFileLoader::loadTextureFromFile(const std::string& textureFileFullName, const int flip, const bool single_channel) {
	std::map<std::string, Texture*>::iterator alreadyLoadedTexture = _alreadyLoadedTextures.find(textureFileFullName);
	Texture * texture = NULL;
	if (alreadyLoadedTexture != _alreadyLoadedTextures.end())
	{
		texture = alreadyLoadedTexture->second;
	}
	else
	{
		texture = new Texture(textureFileFullName.c_str());
		_alreadyLoadedTextures[textureFileFullName] = texture;
	}

	return texture;
}