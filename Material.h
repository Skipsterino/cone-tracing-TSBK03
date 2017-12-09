/**
 * @file	Material.h
 * @Author	Kevin Kjellén
 * @date	2017-05-06
 * @brief	Material representation
 */

#pragma once

#include <glm/glm.hpp>

/**
 * @brief Material representation
 */
class Material
{
public:
	/**
	 * @brief Constructor
	 * @param ambient Ambient Color reflection
	 * @param diffuse Diffuse Color reflection
	 * @param specular Specular Color reflection
	 * @param shininess Specular light spread coefficient
	 */
	Material(
		const glm::vec3& ambient, 
		const glm::vec3& diffuse, 
		const glm::vec3& specular, 
		float shininess,
		float emissivity = 0.f,
		float diffuseReflectivity = 1.f,
		float specularReflectivity = 1.f);

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
	 * @brief Shininess getter
	 * @return Shininess
	 */
	float getShininess() const;

	/**
	* @brief Emissivity getter
	* @return Emissivity
	*/
	float getEmissivity() const;

	/**
	* @brief diffuseReflectivity getter
	* @return diffuseReflectivity
	*/
	float getDiffuseReflectivity() const;

	/**
	* @brief specularReflectivity getter
	* @return specularReflectivity
	*/
	float getSpecularReflectivity() const;

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
	 * @brief Shininess setter
	 * @param value Shininess
	 */
	void setShininess(float value);

	/**
	* @brief Emissivity setter
	* @param value Emissivity
	*/
	void setEmissivity(float value);

	/**
	* @brief diffuseReflectivity setter
	* @param value diffuseReflectivity
	*/
	void setDiffuseReflectivity(float value);

	/**
	* @brief specularReflectivity setter
	* @param value diffuseReflectivity
	*/
	void setSpecularReflectivity(float value);
private:

	/**
	 * @brief Ambient color reflection
	 */
	glm::vec3 ambient;

	/**
	 * @brief Diffuse color reflection
	 */
	glm::vec3 diffuse;
	
	/**
	 * @brief Specular color reflection
	 */
	glm::vec3 specular;
	
	/**
	 * @brief Specular light spread coefficient
	 */
	float shininess;

	/**
	* @brief Emissivity of material
	*/
	float emissivity;

	/**
	* @brief Diffuse reflectivity of material
	*/
	float diffuseReflectivity;

	/**
	* @brief Specular reflectivity of material
	*/
	float specularReflectivity;
};