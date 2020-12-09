#ifndef MTL_MATERIAL_FILE_LOADER
#define MTL_MATERIAL_FILE_LOADER

#include<string>
#include "MaterialFileLoader.h"

class MTLMaterialFileLoader : public MaterialFileLoader
{
public:
	MTLMaterialFileLoader(const std::string materialFilePath, const std::string materialFileName, const std::string texturesPath);
	void loadMaterials(std::vector<Material*>& materials);
private: 
	inline static const std::string MTL_FILE_COMMENT_SIGN = "#";
	inline static const std::string MTL_NEW_MATERIAL_SIGN = "newmtl";
	inline static const std::string SHININESS_SIGN = "Ns";
	inline static const std::string AMBIENT_COLOR_SIGN = "Ka";
	inline static const std::string DIFFUSE_COLOR_SIGN = "Kd";
	inline static const std::string SPECULAR_COLOR_SIGN = "Ks";
	inline static const std::string EMISSION_COLOR_SIGN = "Ke";

	inline static const std::string SHADER_TYPE_SIGN = "shader";
	inline static const std::string IOR_SIGN = "Ni";
	inline static const std::string ROUGNESS_SIGN = "Pr";
	inline static const std::string METALLICNESS_SIGN = "Pm";

	//MAP Textures signs
	inline static const std::string DIFFUSE_COLOR_MAP_SIGN = "map_Kd";
	inline static const std::string SPECULAR_COLOR_MAP_SIGN = "map_Ks";
	inline static const std::string NORMAL_MAP_SIGN = "map_bump";
	inline static const std::string OPACITY_MAP_SIGN = "map_D";
	inline static const std::string ROUGNESS_MAP_SIGN = "map_Pr";
	inline static const std::string METALICNESS_MAP_SIGN = "map_Pm";

	inline static const int MTL_MATERIAL_NAME_POSITION_IN_LINE = 1;

	std::map<std::string, Texture*> _alreadyLoadedTextures;

	Material* _currentMaterial = nullptr;

	Material* readNewMaterial(std::string currentFileLine);
	void trySetColorMtlMaterialProperty(std::string mtlPropertyLine, std::string propertyName, void setterFunction(Material*, Color3f));
	void trySetFloatMtlMaterialProperty(std::string mtlPropertyLine, std::string propertyName, void setterFunction(Material*, float));
	void trySetStringMtlMaterialProperty(std::string mtlPropertyLine, std::string propertyName, void setterFunction(Material*, std::string));
	void trySetIntMtlMaterialProperty(std::string mtlPropertyLine, std::string propertyName, void setterFunction(Material*, int));
	void trySetTextureMtlMaterialProperty(std::string mtlPropertyLine, std::string propertyName, Material::TextureMapSlotNumber slotNumber, void setterFunction(Material*, Texture*, Material::TextureMapSlotNumber));
	void trySetMaterialConstants(std::string currentFileLine);
	void trySetMaterialTextures(std::string currentFileLine);

	Texture* loadTextureFromFile(const std::string& textureFileFullName, const int flip = -1, const bool single_channel = false);

};


#endif