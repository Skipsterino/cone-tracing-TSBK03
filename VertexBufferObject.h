/**
 * @file	VertexBufferObject.h
 * @Author	Joakim Bertils
 * @date	2017-02-12
 * @brief	VBO abstaction for mindless memory allocation and freeing.
 */

#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/**
 * @brief Vertex buffer object abstraction
 */
class VertexBufferObject
{
public:

	/**
	 * @brief Constructor
	 * @param target Target type.
	 */
	explicit VertexBufferObject(GLenum target);

	/**
	 * @brief Destructor.
	 */
	~VertexBufferObject();

	/**
	* @brief Constructor
	*/
	explicit VertexBufferObject(const VertexBufferObject&) = delete;
	
	/**
	* @brief Constructor
	*/
	explicit VertexBufferObject(VertexBufferObject&&) = delete;

	/**
	* @brief Assignment operator
	* @return Ref. to self.
	*/
	const VertexBufferObject& operator=(const VertexBufferObject&) = delete;

	/**
	* @brief Assignment operator
	* @return Ref. to self.
	*/
	VertexBufferObject& operator=(VertexBufferObject&&) = delete;
	
	/**
	 * @brief Binds the VBO for use.
	 */
	void bind();

	/**
	 * @brief Unbinds the VBO.
	 */
	void unbind();

	/**
	 * @brief Stores data in VBO
	 * @param size Size of data
	 * @param data Data
	 * @param usage Data usage hint
	 */
	void storeData(GLuint size, const void* data, GLenum usage);
	
	/**
	 * @brief Setup vertex attrib
	 * @param location Location of attribute
	 * @param elementSize Element size.
	 */
	void setupVertexAttribPointer(GLuint location, GLuint elementSize);

	/**
	 * @brief Getter for handle.
	 * @return OpenGL handle.
	 */
	GLuint getHandle() const;
	
	/**
	 * @brief Getter for current target.
	 * @return Current Target.
	 */
	GLenum getTarget() const;
	
	/**
	 * @brief Getter for data size.
	 * @return Data size.
	 */
	GLuint getSize() const;
private:
	
	/**
	 * @brief OpenGL handle.
	 */
	GLuint vbo;
	
	/**
	 * @brief Current target
	 */
	GLenum target;
	
	/**
	 * @brief Current size
	 */
	GLuint size;
};