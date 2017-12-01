/**
 * @file	TGA.h
 * @Author	Joakim Bertils
 * @date	2017-02-09
 * @brief	Targa File Format (TGA)
 */

#pragma once

#include "TextureFile.h"

/**
 * @brief TGA file representation. 
 */
class TGA : public TextureFile
{
public:
	
	/**
	 * @brief Constructor
	 * @param filePath Path to the file.
	 */
	explicit TGA(const char* filePath);

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
	 * @brief Vector containing the pixels.
	 */
	std::vector<uint8_t> pixels;

	/**
	 * @brief True if the source file is compressed.
	 */
	bool isCompressed;

	/**
	 * @brief The Width of the image.
	 */
	uint32_t width;

	/**
	 * @brief The Height of the image.
	 */
	uint32_t height;

	/**
	 * @brief The Size in bytes of the image.
	 */
	uint32_t size;

	/**
	 * @brief The number of bits per pixel.
	 */
	uint32_t bitsPerPixel;
};