#include "Material.hpp"
#include "Texture.hpp"

Material::Material(void) :
	ka(1.f, 1.f, 1.f),
	kd(1.f, 1.f, 1.f),
	ks(1.f, 1.f, 1.f),
	shininess(32.f),
	diffuseTexture(nullptr),
	specularTexture(nullptr)
{ }

Material::Material(Material const & material)
{
	*this = material;
}

Material & Material::operator=(Material const & material)
{
	ka = material.ka;
	kd = material.kd;
	ks = material.ks;
	shininess = material.shininess;
	diffuseTexture = material.diffuseTexture;
	specularTexture = material.specularTexture;
	return (*this);
}
