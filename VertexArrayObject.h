/**
 * @file	VertexArrayObject.h
 * @Author	Joakim Bertils
 * @date	2017-02-12
 * @brief	VAO abstaction for mindless memory allocation and freeing.
 */

#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/**
 * @brief Vertex Array Object abstraction
 */
class VertexArrayObject
{
public:

	/**
	 * @brief Constructor
	 */
	VertexArrayObject();

	/**
	 * @brief Destructor.
	 */
	~VertexArrayObject();

	/**
	 * @brief Constructor.
	 */
	explicit VertexArrayObject(const VertexArrayObject&) = delete;

	/**
	* @brief Constructor.
	*/
	explicit VertexArrayObject(VertexArrayObject&&) = delete;

	/**
	 * @brief Assignment operator
	 * @return Ref. to self.
	 */
	const VertexArrayObject& operator=(const VertexArrayObject&) = delete;

	/**
	* @brief Assignment operator
	* @return Ref. to self.
	*/
	VertexArrayObject& operator=(VertexArrayObject&&) = delete;

	/**
	 * @brief Bind the VAO for use.
	 */
	void bind();

	/**
	* @brief Unbinds the VAO
	*/
	void unbind();

	/**
	 * @brief Handle getter.
	 * @return OpenGL handle.
	 */
	GLuint getHandle() const;

	/**
	* @brief Handle setter.
	* @param Handle
	*/
	void setHandle(GLuint handle);
private:

	/**
	 * @brief OpenGL handle.
	 */
	GLuint vao;
};