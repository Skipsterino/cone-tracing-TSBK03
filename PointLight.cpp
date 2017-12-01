/**
 * @file	PointLight.cpp
 * @Author	Joakim Bertils
 * @date	2017-05-06
 * @brief	Point Light Representation.
 */

#include "PointLight.h"

PointLight::PointLight(
	const glm::vec3& position,
	const glm::vec3& ambient,
	const glm::vec3& diffuse,
	const glm::vec3& specular,
	float constant,
	float linear,
	float quadratic)
	: position(position),
	constant(constant),
	linear(linear),
	quadratic(quadratic),
	ambient(ambient),
	diffuse(diffuse),
	specular(specular)
{}

glm::vec3 PointLight::getPosition() const
{
	return position;
}

float PointLight::getConstant() const
{
	return constant;
}

float PointLight::getLinear() const
{
	return linear;
}

float PointLight::getQuadratic() const
{
	return quadratic;
}

glm::vec3 PointLight::getAmbient() const
{
	return ambient;
}

glm::vec3 PointLight::getDiffuse() const
{
	return diffuse;
}

glm::vec3 PointLight::getSpecular() const
{
	return specular;
}

void PointLight::setPosition(const glm::vec3& value)
{
	position = value;
}

void PointLight::setConstant(float value)
{
	constant = value;
}

void PointLight::setLinear(float value)
{
	linear = value;
}

void PointLight::setQuadratic(float value)
{
	quadratic = value;
}

void PointLight::setAmbient(const glm::vec3& value)
{
	ambient = value;
}

void PointLight::setDiffuse(const glm::vec3& value)
{
	diffuse = value;
}

void PointLight::setSpecular(const glm::vec3& value)
{
	specular = value;
}

float PointLight::calculateAttenuation(float distance)
{
	return 1.f / (constant + linear*distance + quadratic*distance*distance);
}
