/**
 * @file	RawModel.cpp
 * @Author	Joakim Bertils
 * @date	2017-02-12
 * @brief	Base Class for representing a model in openGL.
 */

#include "RawModel.h"

#include "loadobj.h"

RawModel::RawModel(const char* fileName)
{
	Model* m;

	m = LoadModel(const_cast<char*>(fileName));

	vao.bind();

	vertexPositions.storeData(m->numVertices * 3 * sizeof(GLfloat), m->vertexArray, GL_STATIC_DRAW);
	vertexPositions.setupVertexAttribPointer(0, 3);

	vertexNormals.storeData(m->numVertices * 3 * sizeof(GLfloat), m->normalArray, GL_STATIC_DRAW);
	vertexNormals.setupVertexAttribPointer(1, 3);

	if (m->texCoordArray != nullptr)
	{
		textureCoordinates.storeData(m->numVertices * 2 * sizeof(GLfloat), m->texCoordArray, GL_STATIC_DRAW);
		textureCoordinates.setupVertexAttribPointer(2, 2);
	}

	indexBuffer.storeData(m->numIndices * sizeof(GLuint), m->indexArray, GL_STATIC_DRAW);

	vao.unbind();

	DisposeModel(m);
}

void RawModel::draw()
{
	vao.bind();
	indexBuffer.bind();
	glDrawElements(GL_TRIANGLES, indexBuffer.getSize() / sizeof(GLuint), GL_UNSIGNED_INT, 0L);
	indexBuffer.unbind();
	vao.unbind();
}

RawModel::~RawModel()
{
	// Do nothing
	// VAO and VBO destructor handle stuff
}
