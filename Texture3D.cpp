/**
* @file	Texture3D.cpp
* @Author	Kevin Kjellén
* @date	2017-11-01
* @brief	Class to handle 3D texture used as voxel grid
*/

#include "Texture3D.h"

#include <vector>

Texture3D::Texture3D(const std::vector<GLfloat> & textureBuffer, const int _width, const int _height, const int _depth) :
	width(_width), height(_height), depth(_depth)
{
	// Generate texture on GPU.
	//glEnable(GL_TEXTURE_3D);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_3D, textureID);

	// Parameter options.
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Upload the texture buffer.
	glTexStorage3D(
		GL_TEXTURE_3D,			// texture
		7,						// levels, tweak if necessary
		GL_RGBA8,				// internalformat
		width,					// width
		height,					// heigth
		depth);					// depth

	glTexImage3D(
		GL_TEXTURE_3D,			// target
		0,						// level
		GL_RGBA8,				// internalformat
		width,					// width
		height,					// height
		depth,					// depth
		0,						// border
		GL_RGBA,				// format
		GL_FLOAT,				// type
		&textureBuffer[0]);		// data

	glGenerateMipmap(GL_TEXTURE_3D);
	glBindTexture(GL_TEXTURE_3D, 0);
}

void Texture3D::Clear(GLfloat clearColor[4])
{
	GLint previousBoundTextureID;
	glGetIntegerv(GL_TEXTURE_BINDING_3D, &previousBoundTextureID);
	glBindTexture(GL_TEXTURE_3D, textureID);
	glClearTexImage(textureID, 0, GL_RGBA, GL_FLOAT, &clearColor);
	glBindTexture(GL_TEXTURE_3D, previousBoundTextureID);
}

void Texture3D::bind(GLuint texUnit) const
{
	GLint numTextureUnits;

	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &numTextureUnits);

	if (texUnit >= numTextureUnits)
	{
		throw std::invalid_argument("Requested texture unit larger than supported");
	}

	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(GL_TEXTURE_3D, textureID);
}

