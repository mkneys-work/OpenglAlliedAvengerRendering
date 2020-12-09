#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "vector3.h"
#include "texture.h"

#define NO_TEXTURES 6
#define IOR_AIR 1.000293f
#define IOR_WATER 1.33f
#define IOR_GLASS 1.5f

enum class Shader : char { NORMAL = 1, LAMBERT = 2, PHONG = 3, GLASS = 4, PBR = 5, MIRROR = 6, TS = 7, CT = 8 };
class Material
{
public:	
	enum class TextureMapSlotNumber : char { DIFFUSE = 0, SPECULAR = 1, NORMAL = 2, OPACITY = 3, ROUGNESS = 4, METALLICNESS = 5 };

	Material();
	

	Texture* getTexture(const TextureMapSlotNumber slot);
	std::string getName();
	Shader getShader();
	Color3f getDiffuseColor( const Coord2f* texCoord = nullptr);	
	Color3f getSpecularColor( const Coord2f* texCoord = nullptr);
	float getRoughness(const Coord2f* texCoord = nullptr);
	float getShininess();
	int getMaterialIndex();

	Color3f getEmissionColor();
	Color3f getAmbientColor();

	void setName(const char * name);
	void setName(std::string name);
	void setTexture(const TextureMapSlotNumber slot, Texture* texture);
	void setShader( Shader shader );
	void setAmbientColor(Color3f ambientColor);
	void setDiffuseColor(Color3f diffuseColor);
	void setSpecularColor(Color3f specularColor);
	void setEmissionColor(Color3f emissionColor);
	void setMaterialIndex(int materialIndex);
	void setShininess(float shininess);
	void setIor(float ior);
	void setMetalicness(float metalicness);
	void setRoughness(float roughness);

	

	~Material();
private:
	Color3f _ambientColor;
	Color3f _diffuseColor;
	Color3f _specularColor;
	Color3f _emissionColor;

	float _shininess;
	float _roughness;
	float _metallicness;
	float _reflectivity;
	float _ior;

	int _materialIndex;
	Texture * _textures[NO_TEXTURES];
	
	std::string _name;

	Shader _shader{ Shader::NORMAL };

	Color3f* getTextureTexel(const Coord2f * tex_coord, TextureMapSlotNumber mapSlotNumber);
};

#endif
