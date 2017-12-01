/**
 * @file	PointLight.h
 * @Author	Joakim Bertils
 * @date	2017-05-06
 * @brief	Point Light Representation
 */

#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/**
 * @brief Point light representation
 */
class PointLight
{
public:
	/**
	 * @brief Constructor
	 * @param position 3D position of the light source.
	 * @param ambient Ambient color.
	 * @param diffuse Diffuse color.
	 * @param specular Specular color.
	 * @param constant Constant attenuation coefficient.
	 * @param linear Linear attenuation coefficient.
	 * @param quadratic Quadratic attenuation coefficient
	 */
	PointLight(
		const glm::vec3& position, 
		const glm::vec3& ambient,
		const glm::vec3& diffuse,
		const glm::vec3& specular, 
		float constant = 1.f, 
		float linear = 0.f, 
		float quadratic = 0.f);

	/**
	 * @brief Position getter
	 * @return Position
	 */
	glm::vec3 getPosition() const;

	/**
	 * @brief Constant getter
	 * @return Constant
	 */
	float getConstant() const;

	/**
	 * @brief Linear getter
	 * @return Linear
	 */
	float getLinear() const;

	/**
	 * @brief Quadratic getter
	 * @return Quadratic
	 */
	float getQuadratic() const;

	/**
	 * @brief Ambient getter
	 * @return Ambient
	 */
	glm::vec3 getAmbient() const;

	/**
	 * @brief Diffuse getter
	 * @return Diffuse
	 */
	glm::vec3 getDiffuse() const;

	/**
	 * @brief Specular getter
	 * @return Specular
	 */
	glm::vec3 getSpecular() const;

	/**
	 * @brief Position setter
	 * @param value Position
	 */
	void setPosition(const glm::vec3& value);

	/**
	 * @brief Constant setter
	 * @param value Constant
	 */
	void setConstant(float value);

	/**
	 * @brief Linear setter
	 * @param value Linear
	 */
	void setLinear(float value);

	/**
	 * @brief Quadratic setter
	 * @param value Quadratic
	 */
	void setQuadratic(float value);

	/**
	 * @brief Ambient setter
	 * @param value Ambient
	 */
	void setAmbient(const glm::vec3& value);

	/**
	 * @brief Diffuse setter
	 * @param value Diffuse
	 */
	void setDiffuse(const glm::vec3& value);

	/**
	 * @brief Specular setter
	 * @param value Specular
	 */
	void setSpecular(const glm::vec3& value);

	/**
	 * @brief Calculate Attenuation for a given distance
	 * 
	 * Calculate the attenuation factor 1/p(d) where
	 * p(d) = c + l*d + q*d^2.
	 * 
	 * @param distance The distance to calculate for.
	 * @return Attenuation factor
	 */
	float calculateAttenuation(float distance);
private:
	
	/**
	 * @brief Position of the light source
	 */
	glm::vec3 position;

	
	/**
	 * @brief Constant attenuation factor.
	 */
	float constant;

	/**
	 * @brief Linear attenuation factor.
	 */
	float linear;

	/**
	 * @brief Quadratic attenuation factor.
	 */
	float quadratic;

	/**
	 * @brief Ambient Color.
	 */
	glm::vec3 ambient;

	/**
	 * @brief Diffuse Color.
	 */
	glm::vec3 diffuse;

	/**
	 * @brief Specular Color.
	 */
	glm::vec3 specular;
};