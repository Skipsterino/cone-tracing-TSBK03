/**
* @file	Texture3D.h
* @Author	Kevin Kjellén
* @date	2017-11-01
* @brief	Class to handle 3D texture used as voxel grid
*/

#pragma once

#include <vector>

#include <glew.h>
#include <GLFW/glfw3.h>

// A 3D texture wrapper class
class Texture3D {
public:
	Texture3D(
		const std::vector<GLfloat> & textureBuffer,
		const int width, const int height, const int depth
	);

	unsigned char * textureBuffer = nullptr;
	GLuint textureID;

	// Clears this texture using a given clear color
	void Clear(GLfloat clearColor[4]);

	// Binds the texture to index texUnit
	void bind(GLuint texUnit) const;

private:
	int width, height, depth;
};