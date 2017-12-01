/**
 * @file	Texture2D.cpp
 * @Author	Joakim Bertils
 * @date	2017-05-21
 * @brief	Texture 2D implementation
 */

#include "Texture2D.h"

#include "TGA.h"
#include "BMP.h"

#include <string>
#include <iostream>

Texture2D::Texture2D()
	:width{ 0 }, height{ 0 }, textureID{ 0 } {}

Texture2D::Texture2D(
	const char* filePath,
	TEXTURE_2D_WRAP sWrap,
	TEXTURE_2D_WRAP tWrap,
	TEXTURE_2D_FILTERING magFilter,
	TEXTURE_2D_FILTERING minFilter)
{
	// TODO: Better way of detecting file format.

	TextureFile* file{ nullptr };

	size_t filePathLength = strlen(filePath);

	if (strcmp(filePath + filePathLength - 4, ".tga") == 0) // File is TGA
	{
		file = new TGA{ filePath };
	}
	else if (strcmp(filePath + filePathLength - 4, ".bmp") == 0) // File is Bitmap
	{
		file = new BMP{ filePath };
	}
	else
	{
		throw std::invalid_argument(std::string("Invalid file format. (") + filePath + ")");
	}

	if (!file)
	{
		throw std::invalid_argument(std::string("Invalid file. (") + filePath + ")");
	}

	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(
		GL_TEXTURE_2D,							// Target
		0,										// Level
		file->hasAlpha() ? GL_RGBA : GL_RGB,	// Internal Format
		file->getWidth(),						// Width
		file->getHeight(),						// Height
		0,										// Border
		file->hasAlpha() ? GL_RGBA : GL_RGB,	// Format
		GL_UNSIGNED_BYTE,						// Type
		file->getPixels().data());				// Pixels

	// Setup S coordinate wrap
	switch (sWrap)
	{
	case TEXTURE_2D_WRAP::REPEAT:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		break;
	case TEXTURE_2D_WRAP::MIRRORED_REPEAT:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		break;
	case TEXTURE_2D_WRAP::CLAMP_TO_EDGE:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		break;
	case TEXTURE_2D_WRAP::CLAMP_TO_BORDER:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		break;
	}

	// Setup T coordinate wrap
	switch (tWrap)
	{
	case TEXTURE_2D_WRAP::REPEAT:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		break;
	case TEXTURE_2D_WRAP::MIRRORED_REPEAT:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		break;
	case TEXTURE_2D_WRAP::CLAMP_TO_EDGE:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		break;
	case TEXTURE_2D_WRAP::CLAMP_TO_BORDER:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	}


	switch (magFilter)
	{
	case TEXTURE_2D_FILTERING::NEAREST:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		break;
	case TEXTURE_2D_FILTERING::LINEAR:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	default:
		std::cerr << "Invalid GL_TEXTURE_MAG_FILTER parameter to texture " << filePath << std::endl;
		break;
	}

	switch (minFilter)
	{
	case TEXTURE_2D_FILTERING::NEAREST:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		break;
	case TEXTURE_2D_FILTERING::LINEAR:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		break;
	case TEXTURE_2D_FILTERING::NEAREST_MIPMAP_NEAREST:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		break;
	case TEXTURE_2D_FILTERING::LINEAR_MIPMAP_NEAREST:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		break;
	case TEXTURE_2D_FILTERING::NEAREST_MIPMAP_LINEAR:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		break;
	case TEXTURE_2D_FILTERING::LINEAR_MIPMAP_LINEAR:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		break;
	}

	glGenerateMipmap(GL_TEXTURE_2D);

	width = file->getWidth();
	height = file->getHeight();

	// We are now done with file.
	delete file;
}

Texture2D::Texture2D(GLuint width, GLuint height, TEXTURE_2D_FORMAT format, TEXTURE_2D_DATATYPE type, GLvoid* data)
	: width(width), height(height)
{
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	GLenum f;
	GLenum t;

	switch(format)
	{
	case TEXTURE_2D_FORMAT::DEPTH_COMPONENT:
		f = GL_DEPTH_COMPONENT;
		break;
	case TEXTURE_2D_FORMAT::RGB:
		f = GL_RGB;
		break;
	case TEXTURE_2D_FORMAT::RED:
		f = GL_RED;
		break;
	default:
		throw std::invalid_argument(std::string("Invalid format."));
	}

	switch(type)
	{
	case TEXTURE_2D_DATATYPE::FLOAT: 
		t = GL_FLOAT;
		break;
	case TEXTURE_2D_DATATYPE::UNSIGNED_BYTE:
		t = GL_UNSIGNED_BYTE;
		break;
	default:
		throw std::invalid_argument(std::string("Invalid type."));
	}

	glTexImage2D(
		GL_TEXTURE_2D,	// Target
		0,				// Level
		f,				// Internal format
		width,			// Width
		height,			// Height
		0,				// Border
		f,				// Format
		t,				// Type
		data			// Data
	);
}

Texture2D::~Texture2D()
{
	if (textureID != 0)
		glDeleteTextures(1, &textureID);
}

Texture2D::Texture2D(Texture2D&& other) noexcept
{
	swap(*this, other);
}

GLuint Texture2D::getWidth() const
{
	return width;
}

GLuint Texture2D::getHeight() const
{
	return height;
}

GLuint Texture2D::getHandle() const
{
	return textureID;
}

void Texture2D::setDepthStencilTextureMode(TEXTURE_2D_TEXTURE_MODE mode)
{
	GLint currentTexture;

	glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture);
	glBindTexture(GL_TEXTURE_2D, textureID);

	switch (mode)
	{
	case TEXTURE_2D_TEXTURE_MODE::DEPTH_COMPONENT:
		glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_STENCIL_TEXTURE_MODE, GL_DEPTH_COMPONENT);
		break;
	case TEXTURE_2D_TEXTURE_MODE::STENCIL_INDEX:
		// OBS Fel i OpenGL-manualen. Detta är rätt, inte GL_STENCIL_COMPONENT. 
		// Ref: http://stackoverflow.com/questions/31449740/how-to-utilize-gl-arb-stencil-texturing
		glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_STENCIL_TEXTURE_MODE, GL_STENCIL_INDEX);
		break;
	}

	glBindTexture(GL_TEXTURE_2D, currentTexture);
}

void Texture2D::setTextureBorderColor(Color color)
{
	GLint currentTexture;

	glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Detta är hax men fungerar p.g.a att färgerna ligger i rgba-ordning i color.
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &color.r);

	glBindTexture(GL_TEXTURE_2D, currentTexture);
}

void Texture2D::setTextureBaseLevel(GLint baseLevel)
{
	GLint currentTexture;

	glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, baseLevel);

	glBindTexture(GL_TEXTURE_2D, currentTexture);
}

void Texture2D::setTextureMaxLevel(GLint maxLevel)
{
	GLint currentTexture;

	glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, maxLevel);

	glBindTexture(GL_TEXTURE_2D, currentTexture);
}

void Texture2D::setTextureCompareFunc(TEXTURE_2D_COMPARE_FUNC func)
{
	GLint currentTexture;

	glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture);
	glBindTexture(GL_TEXTURE_2D, textureID);

	switch (func)
	{
	case TEXTURE_2D_COMPARE_FUNC::LESS_EQUAL:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
		break;
	case TEXTURE_2D_COMPARE_FUNC::GREATER_EQUAL:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_GEQUAL);
		break;
	case TEXTURE_2D_COMPARE_FUNC::LESS:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);
		break;
	case TEXTURE_2D_COMPARE_FUNC::GREATER:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_GREATER);
		break;
	case TEXTURE_2D_COMPARE_FUNC::EQUAL:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_EQUAL);
		break;
	case TEXTURE_2D_COMPARE_FUNC::NOT_EQUAL:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_NOTEQUAL);
		break;
	case TEXTURE_2D_COMPARE_FUNC::ALWAYS:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_ALWAYS);
		break;
	case TEXTURE_2D_COMPARE_FUNC::NEVER:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_NEVER);
		break;
	}

	glBindTexture(GL_TEXTURE_2D, currentTexture);
}

void Texture2D::setTextureCompareMode(TEXTURE_2D_COMPARE_MODE mode)
{
	GLint currentTexture;

	glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture);
	glBindTexture(GL_TEXTURE_2D, textureID);

	switch (mode)
	{
	case TEXTURE_2D_COMPARE_MODE::COMPARE_REF_TO_TEXTURE:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
		break;
	case TEXTURE_2D_COMPARE_MODE::NONE:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
		break;
	}

	glBindTexture(GL_TEXTURE_2D, currentTexture);
}

void Texture2D::setTextureLODBias(GLint LODBias)
{
	GLint currentTexture;

	glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, LODBias);

	glBindTexture(GL_TEXTURE_2D, currentTexture);
}

void Texture2D::setTextureMinLOD(GLint minLOD)
{
	GLint currentTexture;

	glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_LOD, minLOD);

	glBindTexture(GL_TEXTURE_2D, currentTexture);
}

void Texture2D::setTextureMaxLOD(GLint maxLOD)
{
	GLint currentTexture;

	glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, maxLOD);

	glBindTexture(GL_TEXTURE_2D, currentTexture);
}

void Texture2D::setTextureWrapS(TEXTURE_2D_WRAP sWrap)
{
	GLint currentTexture;

	glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture);

	glBindTexture(GL_TEXTURE_2D, textureID);

	switch (sWrap)
	{
	case TEXTURE_2D_WRAP::REPEAT:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		break;
	case TEXTURE_2D_WRAP::MIRRORED_REPEAT:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		break;
	case TEXTURE_2D_WRAP::CLAMP_TO_EDGE:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		break;
	case TEXTURE_2D_WRAP::CLAMP_TO_BORDER:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		break;
	}

	glBindTexture(GL_TEXTURE_2D, currentTexture);
}

void Texture2D::setTextureWrapT(TEXTURE_2D_WRAP tWrap)
{
	GLint currentTexture;

	glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture);

	glBindTexture(GL_TEXTURE_2D, textureID);

	switch (tWrap)
	{
	case TEXTURE_2D_WRAP::REPEAT:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		break;
	case TEXTURE_2D_WRAP::MIRRORED_REPEAT:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		break;
	case TEXTURE_2D_WRAP::CLAMP_TO_EDGE:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		break;
	case TEXTURE_2D_WRAP::CLAMP_TO_BORDER:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	}

	glBindTexture(GL_TEXTURE_2D, currentTexture);
}

void Texture2D::setTextureWrapR(TEXTURE_2D_WRAP rWrap)
{
	GLint currentTexture;

	glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture);

	glBindTexture(GL_TEXTURE_2D, textureID);

	switch (rWrap)
	{
	case TEXTURE_2D_WRAP::REPEAT:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		break;
	case TEXTURE_2D_WRAP::MIRRORED_REPEAT:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		break;
	case TEXTURE_2D_WRAP::CLAMP_TO_EDGE:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		break;
	case TEXTURE_2D_WRAP::CLAMP_TO_BORDER:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	}

	glBindTexture(GL_TEXTURE_2D, currentTexture);
}

void Texture2D::setTextureMagFilter(TEXTURE_2D_FILTERING magFilter)
{
	GLint currentTexture;

	glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture);

	glBindTexture(GL_TEXTURE_2D, textureID);

	switch (magFilter)
	{
	case TEXTURE_2D_FILTERING::NEAREST:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		break;
	case TEXTURE_2D_FILTERING::LINEAR:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	}

	glBindTexture(GL_TEXTURE_2D, currentTexture);
}

void Texture2D::setTextureMinFilter(TEXTURE_2D_FILTERING minFilter)
{
	GLint currentTexture;

	glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture);

	glBindTexture(GL_TEXTURE_2D, textureID);

	switch (minFilter)
	{
	case TEXTURE_2D_FILTERING::NEAREST:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		break;
	case TEXTURE_2D_FILTERING::LINEAR:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		break;
	case TEXTURE_2D_FILTERING::NEAREST_MIPMAP_NEAREST:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		break;
	case TEXTURE_2D_FILTERING::LINEAR_MIPMAP_NEAREST:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		break;
	case TEXTURE_2D_FILTERING::NEAREST_MIPMAP_LINEAR:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		break;
	case TEXTURE_2D_FILTERING::LINEAR_MIPMAP_LINEAR:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		break;
	}

	glBindTexture(GL_TEXTURE_2D, currentTexture);
}

void Texture2D::setTextureSwizzleR(TEXTURE_2D_SWIZZLE_COMPONENT comp)
{
	GLint currentTexture;

	glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture);

	glBindTexture(GL_TEXTURE_2D, textureID);

	switch (comp)
	{
	case TEXTURE_2D_SWIZZLE_COMPONENT::RED:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED);
		break;
	case TEXTURE_2D_SWIZZLE_COMPONENT::GREEN:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_GREEN);
		break;
	case TEXTURE_2D_SWIZZLE_COMPONENT::BLUE:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_BLUE);
		break;
	case TEXTURE_2D_SWIZZLE_COMPONENT::ALPHA:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_ALPHA);
		break;
	case TEXTURE_2D_SWIZZLE_COMPONENT::ZERO:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_ZERO);
		break;
	case TEXTURE_2D_SWIZZLE_COMPONENT::ONE:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_ONE);
		break;
	}

	glBindTexture(GL_TEXTURE_2D, currentTexture);
}

void Texture2D::setTextureSwizzleG(TEXTURE_2D_SWIZZLE_COMPONENT comp)
{
	GLint currentTexture;

	glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture);

	glBindTexture(GL_TEXTURE_2D, textureID);

	switch (comp)
	{
	case TEXTURE_2D_SWIZZLE_COMPONENT::RED:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
		break;
	case TEXTURE_2D_SWIZZLE_COMPONENT::GREEN:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_GREEN);
		break;
	case TEXTURE_2D_SWIZZLE_COMPONENT::BLUE:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_BLUE);
		break;
	case TEXTURE_2D_SWIZZLE_COMPONENT::ALPHA:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_ALPHA);
		break;
	case TEXTURE_2D_SWIZZLE_COMPONENT::ZERO:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_ZERO);
		break;
	case TEXTURE_2D_SWIZZLE_COMPONENT::ONE:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_ONE);
		break;
	}

	glBindTexture(GL_TEXTURE_2D, currentTexture);
}

void Texture2D::setTextureSwizzleB(TEXTURE_2D_SWIZZLE_COMPONENT comp)
{
	GLint currentTexture;

	glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture);

	glBindTexture(GL_TEXTURE_2D, textureID);

	switch (comp)
	{
	case TEXTURE_2D_SWIZZLE_COMPONENT::RED:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);
		break;
	case TEXTURE_2D_SWIZZLE_COMPONENT::GREEN:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_GREEN);
		break;
	case TEXTURE_2D_SWIZZLE_COMPONENT::BLUE:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_BLUE);
		break;
	case TEXTURE_2D_SWIZZLE_COMPONENT::ALPHA:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_ALPHA);
		break;
	case TEXTURE_2D_SWIZZLE_COMPONENT::ZERO:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_ZERO);
		break;
	case TEXTURE_2D_SWIZZLE_COMPONENT::ONE:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_ONE);
		break;
	}

	glBindTexture(GL_TEXTURE_2D, currentTexture);
}

void Texture2D::setTextureSwizzleA(TEXTURE_2D_SWIZZLE_COMPONENT comp)
{
	GLint currentTexture;

	glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture);

	glBindTexture(GL_TEXTURE_2D, textureID);

	switch (comp)
	{
	case TEXTURE_2D_SWIZZLE_COMPONENT::RED:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_RED);
		break;
	case TEXTURE_2D_SWIZZLE_COMPONENT::GREEN:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_GREEN);
		break;
	case TEXTURE_2D_SWIZZLE_COMPONENT::BLUE:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_BLUE);
		break;
	case TEXTURE_2D_SWIZZLE_COMPONENT::ALPHA:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_ALPHA);
		break;
	case TEXTURE_2D_SWIZZLE_COMPONENT::ZERO:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_ZERO);
		break;
	case TEXTURE_2D_SWIZZLE_COMPONENT::ONE:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_ONE);
		break;
	}

	glBindTexture(GL_TEXTURE_2D, currentTexture);
}

TEXTURE_2D_TEXTURE_MODE Texture2D::getDepthStencilTextureMode() const
{
	GLint mode;
	TEXTURE_2D_TEXTURE_MODE returnVal;
	GLint currentTexture;

	// Save the currently bound texture
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glGetTexParameteriv(GL_TEXTURE_2D, GL_DEPTH_STENCIL_TEXTURE_MODE, &mode);

	switch (static_cast<GLuint>(mode))
	{
	case GL_DEPTH_COMPONENT:
		returnVal = TEXTURE_2D_TEXTURE_MODE::DEPTH_COMPONENT;
		break;
	case GL_STENCIL_INDEX:
		returnVal = TEXTURE_2D_TEXTURE_MODE::STENCIL_INDEX;
		break;
	default:
		returnVal = TEXTURE_2D_TEXTURE_MODE::INVALID_VALUE;
	}

	glBindTexture(GL_TEXTURE_2D, currentTexture);

	return returnVal;
}

Color Texture2D::getTextureBorderColor() const
{
	float color[4];
	Color returnVal;
	GLint currentTexture;

	glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glGetTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);

	returnVal = Color(color);

	glBindTexture(GL_TEXTURE_2D, currentTexture);

	return returnVal;
}

GLint Texture2D::getTextureBaseLevel() const
{
	GLint returnVal;
	GLint currentTexture;

	glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, &returnVal);

	glBindTexture(GL_TEXTURE_2D, currentTexture);

	return returnVal;
}

GLint Texture2D::getTextureMaxLevel() const
{
	GLint returnVal;
	GLint currentTexture;

	glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, &returnVal);

	glBindTexture(GL_TEXTURE_2D, currentTexture);

	return returnVal;
}

TEXTURE_2D_COMPARE_FUNC Texture2D::getTextureCompareFunc() const
{
	GLint compareFunc;
	TEXTURE_2D_COMPARE_FUNC returnVal;
	GLint currentTexture;

	glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, &compareFunc);

	switch (compareFunc)
	{
	case GL_LEQUAL:
		returnVal = TEXTURE_2D_COMPARE_FUNC::LESS_EQUAL;
		break;
	case GL_GEQUAL:
		returnVal = TEXTURE_2D_COMPARE_FUNC::GREATER_EQUAL;
		break;
	case GL_LESS:
		returnVal = TEXTURE_2D_COMPARE_FUNC::LESS;
		break;
	case GL_GREATER:
		returnVal = TEXTURE_2D_COMPARE_FUNC::GREATER;
		break;
	case GL_EQUAL:
		returnVal = TEXTURE_2D_COMPARE_FUNC::EQUAL;
		break;
	case GL_NOTEQUAL:
		returnVal = TEXTURE_2D_COMPARE_FUNC::NOT_EQUAL;
		break;
	case GL_ALWAYS:
		returnVal = TEXTURE_2D_COMPARE_FUNC::ALWAYS;
		break;
	case GL_NEVER:
		returnVal = TEXTURE_2D_COMPARE_FUNC::NEVER;
		break;
	default:
		returnVal = TEXTURE_2D_COMPARE_FUNC::INVALID_VALUE;
	}

	glBindTexture(GL_TEXTURE_2D, currentTexture);

	return returnVal;
}

TEXTURE_2D_COMPARE_MODE Texture2D::getTextureCompareMode() const
{
	GLint mode;
	TEXTURE_2D_COMPARE_MODE returnVal;
	GLint currentTexture;

	glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, &mode);

	switch (mode)
	{
	case GL_COMPARE_REF_TO_TEXTURE:
		returnVal = TEXTURE_2D_COMPARE_MODE::COMPARE_REF_TO_TEXTURE;
		break;
	case GL_NONE:
		returnVal = TEXTURE_2D_COMPARE_MODE::NONE;
		break;
	default:
		returnVal = TEXTURE_2D_COMPARE_MODE::INVALID_VALUE;
	}

	glBindTexture(GL_TEXTURE_2D, currentTexture);

	return returnVal;
}

GLint Texture2D::getTextureLODBias() const
{
	GLint returnVal;
	GLint currentTexture;

	glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, &returnVal);

	glBindTexture(GL_TEXTURE_2D, currentTexture);

	return returnVal;
}

GLint Texture2D::getTextureMinLOD() const
{
	GLint returnVal;
	GLint currentTexture;

	glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MIN_LOD, &returnVal);

	glBindTexture(GL_TEXTURE_2D, currentTexture);

	return returnVal;
}

GLint Texture2D::getTextureMaxLOD() const
{
	GLint returnVal;
	GLint currentTexture;

	glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, &returnVal);

	glBindTexture(GL_TEXTURE_2D, currentTexture);

	return returnVal;
}

TEXTURE_2D_WRAP Texture2D::getTextureWrapS() const
{
	GLint sWrap;
	TEXTURE_2D_WRAP returnVal;
	GLint currentTexture;

	// Save the currently bound texture
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, &sWrap);

	switch (static_cast<GLuint>(sWrap))
	{
	case GL_REPEAT:
		returnVal = TEXTURE_2D_WRAP::REPEAT;
		break;
	case GL_MIRRORED_REPEAT:
		returnVal = TEXTURE_2D_WRAP::MIRRORED_REPEAT;
		break;
	case GL_CLAMP_TO_EDGE:
		returnVal = TEXTURE_2D_WRAP::CLAMP_TO_EDGE;
		break;
	case GL_CLAMP_TO_BORDER:
		returnVal = TEXTURE_2D_WRAP::CLAMP_TO_BORDER;
		break;
	default:
		returnVal = TEXTURE_2D_WRAP::INVALID_VALUE;
	}

	glBindTexture(GL_TEXTURE_2D, currentTexture);

	return returnVal;
}

TEXTURE_2D_WRAP Texture2D::getTextureWrapT() const
{
	GLint tWrap;
	TEXTURE_2D_WRAP returnVal;
	GLint currentTexture;

	// Save the currently bound texture
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, &tWrap);

	switch (static_cast<GLuint>(tWrap))
	{
	case GL_REPEAT:
		returnVal = TEXTURE_2D_WRAP::REPEAT;
		break;
	case GL_MIRRORED_REPEAT:
		returnVal = TEXTURE_2D_WRAP::MIRRORED_REPEAT;
		break;
	case GL_CLAMP_TO_EDGE:
		returnVal = TEXTURE_2D_WRAP::CLAMP_TO_EDGE;
		break;
	case GL_CLAMP_TO_BORDER:
		returnVal = TEXTURE_2D_WRAP::CLAMP_TO_BORDER;
		break;
	default:
		return TEXTURE_2D_WRAP::INVALID_VALUE;
	}

	glBindTexture(GL_TEXTURE_2D, currentTexture);

	return returnVal;
}

TEXTURE_2D_WRAP Texture2D::getTextureWrapR() const
{
	GLint rWrap;
	TEXTURE_2D_WRAP returnVal;
	GLint currentTexture;

	// Save the currently bound texture
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, &rWrap);

	switch (static_cast<GLuint>(rWrap))
	{
	case GL_REPEAT:
		returnVal = TEXTURE_2D_WRAP::REPEAT;
		break;
	case GL_MIRRORED_REPEAT:
		returnVal = TEXTURE_2D_WRAP::MIRRORED_REPEAT;
		break;
	case GL_CLAMP_TO_EDGE:
		returnVal = TEXTURE_2D_WRAP::CLAMP_TO_EDGE;
		break;
	case GL_CLAMP_TO_BORDER:
		returnVal = TEXTURE_2D_WRAP::CLAMP_TO_BORDER;
		break;
	default:
		return TEXTURE_2D_WRAP::INVALID_VALUE;
	}

	glBindTexture(GL_TEXTURE_2D, currentTexture);

	return returnVal;
}

TEXTURE_2D_FILTERING Texture2D::getTextureMagFilter() const
{
	GLint magFilter;
	TEXTURE_2D_FILTERING returnVal;
	GLint currentTexture;

	// Save the currently bound texture
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, &magFilter);

	switch (static_cast<GLuint>(magFilter))
	{
	case GL_NEAREST:
		returnVal = TEXTURE_2D_FILTERING::NEAREST;
		break;
	case GL_LINEAR:
		returnVal = TEXTURE_2D_FILTERING::LINEAR;
		break;
	default:
		returnVal = TEXTURE_2D_FILTERING::INVALID_VALUE;
	}

	glBindTexture(GL_TEXTURE_2D, currentTexture);

	return returnVal;
}

TEXTURE_2D_FILTERING Texture2D::getTextureMinFilter() const
{
	GLint minFilter;
	TEXTURE_2D_FILTERING returnVal;
	GLint currentTexture;

	// Save the currently bound texture
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, &minFilter);

	switch (static_cast<GLuint>(minFilter))
	{
	case GL_NEAREST:
		returnVal = TEXTURE_2D_FILTERING::NEAREST;
		break;
	case GL_LINEAR:
		returnVal = TEXTURE_2D_FILTERING::LINEAR;
		break;
	case GL_NEAREST_MIPMAP_NEAREST:
		returnVal = TEXTURE_2D_FILTERING::NEAREST_MIPMAP_NEAREST;
		break;
	case GL_LINEAR_MIPMAP_NEAREST:
		returnVal = TEXTURE_2D_FILTERING::LINEAR_MIPMAP_NEAREST;
		break;
	case GL_NEAREST_MIPMAP_LINEAR:
		returnVal = TEXTURE_2D_FILTERING::NEAREST_MIPMAP_LINEAR;
		break;
	case GL_LINEAR_MIPMAP_LINEAR:
		returnVal = TEXTURE_2D_FILTERING::LINEAR_MIPMAP_LINEAR;
		break;
	default:
		returnVal = TEXTURE_2D_FILTERING::INVALID_VALUE;
	}

	glBindTexture(GL_TEXTURE_2D, currentTexture);

	return returnVal;
}

TEXTURE_2D_SWIZZLE_COMPONENT Texture2D::getTextureSwizzleR() const
{
	GLint comp;
	TEXTURE_2D_SWIZZLE_COMPONENT returnVal;
	GLint currentTexture;

	// Save the currently bound texture
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, &comp);

	switch (comp)
	{
	case GL_RED:
		returnVal = TEXTURE_2D_SWIZZLE_COMPONENT::RED;
		break;
	case GL_GREEN:
		returnVal = TEXTURE_2D_SWIZZLE_COMPONENT::GREEN;
		break;
	case GL_BLUE:
		returnVal = TEXTURE_2D_SWIZZLE_COMPONENT::BLUE;
		break;
	case GL_ALPHA:
		returnVal = TEXTURE_2D_SWIZZLE_COMPONENT::ALPHA;
		break;
	case GL_ZERO:
		returnVal = TEXTURE_2D_SWIZZLE_COMPONENT::ZERO;
		break;
	case GL_ONE:
		returnVal = TEXTURE_2D_SWIZZLE_COMPONENT::ONE;
		break;
	default:
		returnVal = TEXTURE_2D_SWIZZLE_COMPONENT::INVALID_VALUE;
	}

	glBindTexture(GL_TEXTURE_2D, currentTexture);

	return returnVal;
}

TEXTURE_2D_SWIZZLE_COMPONENT Texture2D::getTextureSwizzleG() const
{
	GLint comp;
	TEXTURE_2D_SWIZZLE_COMPONENT returnVal;
	GLint currentTexture;

	// Save the currently bound texture
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, &comp);

	switch (comp)
	{
	case GL_RED:
		returnVal = TEXTURE_2D_SWIZZLE_COMPONENT::RED;
		break;
	case GL_GREEN:
		returnVal = TEXTURE_2D_SWIZZLE_COMPONENT::GREEN;
		break;
	case GL_BLUE:
		returnVal = TEXTURE_2D_SWIZZLE_COMPONENT::BLUE;
		break;
	case GL_ALPHA:
		returnVal = TEXTURE_2D_SWIZZLE_COMPONENT::ALPHA;
		break;
	case GL_ZERO:
		returnVal = TEXTURE_2D_SWIZZLE_COMPONENT::ZERO;
		break;
	case GL_ONE:
		returnVal = TEXTURE_2D_SWIZZLE_COMPONENT::ONE;
		break;
	default:
		returnVal = TEXTURE_2D_SWIZZLE_COMPONENT::INVALID_VALUE;
	}

	glBindTexture(GL_TEXTURE_2D, currentTexture);

	return returnVal;
}

TEXTURE_2D_SWIZZLE_COMPONENT Texture2D::getTextureSwizzleB() const
{
	GLint comp;
	TEXTURE_2D_SWIZZLE_COMPONENT returnVal;
	GLint currentTexture;

	// Save the currently bound texture
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, &comp);

	switch (comp)
	{
	case GL_RED:
		returnVal = TEXTURE_2D_SWIZZLE_COMPONENT::RED;
		break;
	case GL_GREEN:
		returnVal = TEXTURE_2D_SWIZZLE_COMPONENT::GREEN;
		break;
	case GL_BLUE:
		returnVal = TEXTURE_2D_SWIZZLE_COMPONENT::BLUE;
		break;
	case GL_ALPHA:
		returnVal = TEXTURE_2D_SWIZZLE_COMPONENT::ALPHA;
		break;
	case GL_ZERO:
		returnVal = TEXTURE_2D_SWIZZLE_COMPONENT::ZERO;
		break;
	case GL_ONE:
		returnVal = TEXTURE_2D_SWIZZLE_COMPONENT::ONE;
		break;
	default:
		returnVal = TEXTURE_2D_SWIZZLE_COMPONENT::INVALID_VALUE;
	}

	glBindTexture(GL_TEXTURE_2D, currentTexture);

	return returnVal;
}

TEXTURE_2D_SWIZZLE_COMPONENT Texture2D::getTextureSwizzleA() const
{
	GLint comp;
	TEXTURE_2D_SWIZZLE_COMPONENT returnVal;
	GLint currentTexture;

	// Save the currently bound texture
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, &comp);

	switch (comp)
	{
	case GL_RED:
		returnVal = TEXTURE_2D_SWIZZLE_COMPONENT::RED;
		break;
	case GL_GREEN:
		returnVal = TEXTURE_2D_SWIZZLE_COMPONENT::GREEN;
		break;
	case GL_BLUE:
		returnVal = TEXTURE_2D_SWIZZLE_COMPONENT::BLUE;
		break;
	case GL_ALPHA:
		returnVal = TEXTURE_2D_SWIZZLE_COMPONENT::ALPHA;
		break;
	case GL_ZERO:
		returnVal = TEXTURE_2D_SWIZZLE_COMPONENT::ZERO;
		break;
	case GL_ONE:
		returnVal = TEXTURE_2D_SWIZZLE_COMPONENT::ONE;
		break;
	default:
		returnVal = TEXTURE_2D_SWIZZLE_COMPONENT::INVALID_VALUE;
	}

	glBindTexture(GL_TEXTURE_2D, currentTexture);

	return returnVal;
}

void Texture2D::bind() const
{
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture2D::bind(GLuint texUnit) const
{
	GLint numTextureUnits;

	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &numTextureUnits);

	if (texUnit >= numTextureUnits)
	{
		throw std::invalid_argument("Requested texture unit larger than supported");
	}

	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(GL_TEXTURE_2D, textureID);
}

void swap(Texture2D& first, Texture2D& second) noexcept
{
	std::swap(first.textureID, second.textureID);
	std::swap(first.width, second.width);
	std::swap(first.height, second.height);
}