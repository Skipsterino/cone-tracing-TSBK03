/**
 * @file	TextureFile.h
 * @Author	Joakim Bertils
 * @date	2017-02-09
 * @brief	Texture File Interface
 */

#pragma once

#include <vector>
#include "PixelInfo.h"

/**
 * @brief Abstract base class for representing a texture file
 */
class TextureFile
{
public:

	/**
	 * @brief Virtual destructor 
	 */
	virtual ~TextureFile() = default;

	/**
	 * @brief Gets the pixels in the file.
	 * @return Vector containing pixel data.
	 */
	virtual const std::vector<uint8_t>& getPixels() const = 0;

	/**
	 * @brief Gets the width of the texture
	 * @return Width of texture
	 */
	virtual uint32_t getWidth() const = 0;

	/**
	 * @brief Gets the height of the texture
	 * @return Height of texture
	 */
	virtual uint32_t getHeight() const = 0;

	/**
	 * @brief Checks whether the file has alpha channel information.
	 * @return True if file has alpha information.
	 */
	virtual bool hasAlpha() const = 0;
};