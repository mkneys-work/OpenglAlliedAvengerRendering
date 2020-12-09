#include "pch.h"
#include "material.h"

Material::Material()
{
	_ambientColor = Color3f{ 0.1f, 0.1f, 0.1f };
	_diffuseColor = Color3f{ 0.5f, 0.5f, 0.5f };
	_specularColor = Color3f{ 0.6f, 0.6f, 0.6f };

	_emissionColor = Color3f{ 0.0f, 0.0f, 0.0f };

	_reflectivity = static_cast<float>(0.99);
	_shininess = 0.0f;
	_roughness = 1.0f;
	_metallicness = 0.0f;

	_ior = -1.0f;

	memset(_textures, 0, sizeof(*_textures) * NO_TEXTURES);

	_name = "default";
	_shader = Shader::PHONG;
}

Material::~Material()
{
	for ( int i = 0; i < NO_TEXTURES; ++i )
	{
		if ( _textures[i] )
		{
			delete[] _textures[i];
			_textures[i] = nullptr;
		};
	}
}

void Material::setName( const char* name )
{
	_name = std::string( name );
}

void Material::setName(const std::string name)
{
	_name = name;
}

void Material::setMaterialIndex(int materialIndex) {
	_materialIndex = materialIndex;
}

std::string Material::getName()
{
	return _name;
}

void Material::setTexture(const TextureMapSlotNumber slot, Texture * texture )
{
	_textures[static_cast<char>(slot)] = texture;
}

Texture* Material::getTexture(const TextureMapSlotNumber slot)
{
	return _textures[static_cast<char>(slot)];
}

Shader Material::getShader()
{
	return _shader;
}

void Material::setShader( Shader shader )
{
	_shader = shader;
}

Color3f Material::getAmbientColor()
{
	return _ambientColor;
}

float Material::getShininess()
{
	return _shininess;
}

int Material::getMaterialIndex() {
	return _materialIndex;
}

Color3f Material::getDiffuseColor( const Coord2f * tex_coord )
{
	Color3f* texel = getTextureTexel(tex_coord, TextureMapSlotNumber::DIFFUSE);
	return texel == nullptr ? _diffuseColor : *texel;
}

Color3f Material::getSpecularColor( const Coord2f * tex_coord )
{
	Color3f* texel = getTextureTexel(tex_coord, TextureMapSlotNumber::SPECULAR);
	return texel == nullptr ? _specularColor : *texel;
}

float Material::getRoughness( const Coord2f* tex_coord)
{
	Color3f* texel = getTextureTexel(tex_coord, TextureMapSlotNumber::ROUGNESS);
	return texel == nullptr ? _roughness : texel->r;
}

Color3f* Material::getTextureTexel(const Coord2f * tex_coord, TextureMapSlotNumber mapSlotNumber) {
	if (tex_coord)
	{
		Texture* texture = _textures[static_cast<char>(mapSlotNumber)];

		if (texture)
		{
			return &texture->getTexel(tex_coord->u, tex_coord->v, false);
		}
	}
	
	return nullptr;
}

void Material::setAmbientColor(Color3f ambientColor) {
	_ambientColor = ambientColor;
}

void Material::setDiffuseColor(Color3f diffuseColor) {
	_diffuseColor = diffuseColor;
}

void Material::setSpecularColor(Color3f specularColor) {
	_specularColor = specularColor;
}

void Material::setEmissionColor(Color3f emissionColor) {
	_emissionColor = emissionColor;
}

void Material::setShininess(float shininess) {
	_shininess = shininess;
}

void Material::setIor(float ior) {
	_ior = ior;
}

void Material::setMetalicness(float metalicness) {
	_metallicness = metalicness;
}

void Material::setRoughness(float roughness) {
	_roughness = roughness;
}

Color3f Material::getEmissionColor()
{
	return _emissionColor;
}
