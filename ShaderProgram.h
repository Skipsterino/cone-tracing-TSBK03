/**
 * @file	ShaderProgram.h
 * @Author	Joakim Bertils
 * @date	2017-02-09
 * @brief	Shader Program interface
 */

#pragma once

#include <string>
#include <stdexcept>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "PointLight.h"
#include "Material.h"

/**
 * @brief Shader Program Exception
 */
class ShaderProgramException : public std::runtime_error
{
public:
	using std::runtime_error::runtime_error;
};

/**
 * @brief Representation of a complete shader program.
 */
class ShaderProgram
{
public:

	/**
	 * @brief Default constructor.
	 * @note Deleted. 
	 */
	ShaderProgram() = delete;

	/**
	 * @brief Constructor for the shader program.
	 * @param vertexShaderPath Path to the vertex shader source.
	 * @param fragmentShaderPath Path to the fragment shader source.
	 * @param geometryShaderPath Path to the geometry shader source code. If equal to "", the geometry shader is not included.
	 */
	ShaderProgram(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& geometryShaderPath = "");

	/**
	 * @brief Destructor.
	 * 
	 * Handles the memory cleanup for the shaders.
	 */
	~ShaderProgram();

	/**
	 * @brief Compile Shader
	 * 
	 * Compiles the vertex and fragment shader. It does not link the program.
	 */
	void compile();

	/**
	 * @brief Link Shader
	 * 
	 * Links the shader. Expects the vertex and fragment shaders to be loaded and compiled beforehand.
	 */
	void link() const;

	/**
	 * @brief Binds an attribute location for the shader.
	 * @param index Location of the attribute.
	 * @param name Name of the attribute.
	 */
	void bindAttribLocation(GLuint index, const GLchar* name) const;

	/**
	 * @brief Sets the openGL pipeline to use the shader for subsequent draws.
	 */
	void use() const;

	/**
	 * @brief Disables the shader.
	 */
	void disable() const;

	/**
	 * @brief Gets the openGL shader program handle.
	 * @return Program Handle.
	 */
	GLuint getShaderProgramHandle() const;

	/**
	 * @brief Uploads a value as an uniform to the shader.
	 * @param name Name of the uniform.
	 * @param value Value to be upload.
	 */
	void uploadUniform(const std::string& name, float value);

	/**
	* @brief Uploads a value as an uniform to the shader.
	* @param name Name of the uniform.
	* @param value Value to be upload.
	*/
	void uploadUniform(const std::string& name, int value);

	/**
	* @brief Uploads a value as an uniform to the shader.
	* @param name Name of the uniform.
	* @param value Value to be upload.
	*/
	void uploadUniform(const std::string& name, glm::vec2 value);

	/**
	* @brief Uploads a value as an uniform to the shader.
	* @param name Name of the uniform.
	* @param value Value to be upload.
	*/
	void uploadUniform(const std::string& name, glm::vec3 value);

	/**
	* @brief Uploads a value as an uniform to the shader.
	* @param name Name of the uniform.
	* @param value Value to be upload.
	*/
	void uploadUniform(const std::string& name, glm::vec4 value);

	/**
	* @brief Uploads a value as an uniform to the shader.
	* @param name Name of the uniform.
	* @param value Value to be upload.
	*/
	void uploadUniform(const std::string& name, glm::mat4 value);

	/**
	 * @brief Uploads a value as an uniform to the shader
	 * @param name Name of the uniform
	 * @param pointLight PointLight object
	 */
	void uploadUniform(const std::string& name, const PointLight& pointLight);

	/**
	 * @brief Uploads a value as an uniform to the shader
	 * @param name Name of the uniform
	 * @param material Material object
	 */
	void uploadUniform(const std::string& name, const Material& material);

	/**
	 * @brief Swaps two shaders
	 * @param lhs First shader
	 * @param rhs Second Shader
	 */
	friend void swap(ShaderProgram& lhs, ShaderProgram& rhs) noexcept;
private:

	/**
	 * @brief Path to the vertex shader source.
	 */
	std::string vertexShaderPath;

	/**
	 * @brief Path to the fragment shader source.
	 */
	std::string fragmentShaderPath;

	/**
	* @brief Path to the geometry shader source.
	*/
	std::string geometryShaderPath;

	/**
	 * @brief OpenGL shader program handle.
	 */
	GLuint shaderProgramHandle;

	/**
	 * @brief OpenGL vertex shader handle.
	 */
	GLuint vertexShaderHandle;

	/**
	 * @brief OpenGL fragment shader handle.
	 */
	GLuint fragmentShaderHandle;

	/**
	* @brief OpenGL geometry shader handle.
	*/
	GLuint geometryShaderHandle;
};