/**
 * @file	Material.cpp
 * @Author	Joakim Bertils
 * @date	2017-05-06
 * @brief	Material representation
 */

#include "Material.h"

Material::Material(
	const glm::vec3& ambient,
	const glm::vec3& diffuse,
	const glm::vec3& specular,
	float shininess,
	float emissivity,
	float diffuseReflectivity,
	float specularReflectivity)
	:
	ambient(ambient),
	diffuse(diffuse),
	specular(specular),
	shininess(shininess),
	emissivity(emissivity),
	diffuseReflectivity(diffuseReflectivity),
	specularReflectivity(specularReflectivity) {}

glm::vec3 Material::getAmbient() const
{
	return ambient;
}

glm::vec3 Material::getDiffuse() const
{
	return diffuse;
}

glm::vec3 Material::getSpecular() const
{
	return specular;
}

float Material::getShininess() const
{
	return shininess;
}

float Material::getEmissivity() const
{
	return emissivity;
}

float Material::getDiffuseReflectivity() const
{
	return diffuseReflectivity;
}

float Material::getSpecularReflectivity() const
{
	return specularReflectivity;
}

void Material::setAmbient(const glm::vec3& value)
{
	ambient = value;
}

void Material::setDiffuse(const glm::vec3& value)
{
	diffuse = value;
}

void Material::setSpecular(const glm::vec3& value)
{
	specular = value;
}

void Material::setShininess(float value)
{
	shininess = value;
}

void Material::setEmissivity(float value)
{
	emissivity = value;
}

void Material::setDiffuseReflectivity(float value)
{
	diffuseReflectivity = value;
}

void Material::setSpecularReflectivity(float value)
{
	specularReflectivity = value;
}
