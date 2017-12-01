/**
 * @file	RawModel.h
 * @Author	Joakim Bertils
 * @date	2017-02-12
 * @brief	Base Class for representing a model in openGL.
 */

#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "VertexArrayObject.h"
#include "VertexBufferObject.h"
#include "ShaderProgram.h"

/**
 * @brief Raw Model base class
 */
class RawModel
{
public:
	/**
	 * @brief Constructor
	 * @param fileName File Path of model file.
	 */
	explicit RawModel(const char* fileName);

	/**
	 * @brief Draws the model to the current context.
	 */
	virtual void draw();

	/**
	 * @brief Destructor.
	 */
	virtual ~RawModel();
protected:

	/**
	 * @brief Model VAO
	 */
	VertexArrayObject vao{};

	/**
	 * @brief Position VBO
	 */
	VertexBufferObject vertexPositions{ GL_ARRAY_BUFFER };

	/**
	 * @brief Normals VBO
	 */
	VertexBufferObject vertexNormals{ GL_ARRAY_BUFFER };

	/**
	 * @brief TexCoords VBO
	 */
	VertexBufferObject textureCoordinates{ GL_ARRAY_BUFFER };

	/**
	 * @brief IndexBuffer VBO
	 */
	VertexBufferObject indexBuffer{ GL_ELEMENT_ARRAY_BUFFER };
};