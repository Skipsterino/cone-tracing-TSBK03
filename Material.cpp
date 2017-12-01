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
	float emissivity)
	:
	ambient(ambient),
	diffuse(diffuse),
	specular(specular),
	shininess(shininess),
	emissivity(emissivity) {}

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
