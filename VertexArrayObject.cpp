/**
 * @file	VertexArrayObject.cpp
 * @Author	Joakim Bertils
 * @date	2017-02-12
 * @brief	VAO abstaction for mindless memory allocation and freeing.
 */

#include "VertexArrayObject.h"

VertexArrayObject::VertexArrayObject()
{
	glGenVertexArrays(1, &vao);
}

VertexArrayObject::~VertexArrayObject()
{
	glDeleteVertexArrays(1, &vao);
}

void VertexArrayObject::bind()
{
	glBindVertexArray(vao);
}

void VertexArrayObject::unbind()
{
	glBindVertexArray(0);
}

GLuint VertexArrayObject::getHandle() const
{
	return vao;
}

void VertexArrayObject::setHandle(GLuint handle)
{
	vao = handle;
}
