/**
 * @file	BMP.h
 * @Author	Joakim Bertils
 * @date	2017-02-09
 * @brief	Implementation of bitmap class
 */

#pragma once

#include <vector>
#include "TextureFile.h"

/**
 * @brief BMP (Bitmap) representation. 
 */
class BMP : public TextureFile
{
public:

	/**
	 * @brief Constructor
	 * @param filePath Path to the bmp file.
	 */
	explicit BMP(const char* filePath);

	/**
	* @brief Gets the pixels in the file.
	* @return Vector containing pixel data.
	*/
	const std::vector<uint8_t>& getPixels() const override;

	/**
	* @brief Gets the width of the texture
	* @return Width of texture
	*/
	uint32_t getWidth() const override;

	/**
	* @brief Gets the height of the texture
	* @return Height of texture
	*/
	uint32_t getHeight() const override;

	/**
	* @brief Checks whether the file has alpha channel information.
	* @return True if file has alpha information.
	*/
	bool hasAlpha() const override;

private:

	/**
	 * @brief The width of the image.
	 */
	uint32_t width;

	/**
	 * @brief The Height of the image.
	 */
	uint32_t height;

	/**
	 * @brief Number of bits per pixel.
	 */
	uint16_t bitsPerPixel;

	/**
	 * @brief The vector containing all pixel data.
	 */
	std::vector<uint8_t> pixels;
};