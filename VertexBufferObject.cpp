/**
 * @file	VertexBufferObject.cpp
 * @Author	Joakim Bertils
 * @date	2017-02-12
 * @brief	VBO abstaction for mindless memory allocation and freeing.
 */

#include "VertexBufferObject.h"

VertexBufferObject::VertexBufferObject(GLenum target)
	: target{ target }
{
	glGenBuffers(1, &vbo);
}

VertexBufferObject::~VertexBufferObject()
{
	glDeleteBuffers(1, &vbo);
}

void VertexBufferObject::bind()
{
	glBindBuffer(target, vbo);
}

void VertexBufferObject::unbind()
{
	glBindBuffer(target, 0);
}

void VertexBufferObject::storeData(GLuint size, const void* data, GLenum usage)
{
	bind();
	glBufferData(target, size, data, usage);
	this->size = size;
	unbind();
}

void VertexBufferObject::setupVertexAttribPointer(GLuint location, GLuint elementSize)
{
	bind();
	glVertexAttribPointer(location, elementSize, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(location);
	unbind();
}

GLuint VertexBufferObject::getHandle() const
{
	return vbo;
}

GLenum VertexBufferObject::getTarget() const
{
	return target;
}

GLuint VertexBufferObject::getSize() const
{
	return size;
}
