/**
 * @file	BMP.cpp
 * @Author	Joakim Bertils
 * @date	2017-05-09
 * @brief	Bitmap Class
 */

#include "BMP.h"

#include <fstream>
#include <string>


 /**
  * @brief Macro for bitcount of 1.
  */
#define BI_BITCOUNT_1			1

  /**
  * @brief Macro for bitcount of 4.
  */
#define BI_BITCOUNT_4			4

  /**
  * @brief Macro for bitcount of 8.
  */
#define BI_BITCOUNT_8			8

  /**
  * @brief Macro for bitcount of 16.
  */
#define BI_BITCOUNT_16			16

  /**
  * @brief Macro for bitcount of 24.
  */
#define BI_BITCOUNT_24			24

  /**
  * @brief Macro for bitcount of 32.
  */
#define BI_BITCOUNT_32			32

  /**
  * @brief Macro for no compression.
  */
#define BI_COMPRESSION_NONE		0

  /**
  * @brief Macro for RLE8 compression.
  */
#define BI_COMPRESSION_RLE8		1

  /**
  * @brief Macro for RLE4 compression.
  */
#define BI_COMPRESSION_RLE4		2

  /**
  * @brief Macro for bitfield compression.
  */
#define BI_COMPRESSION_BITFIELD	3

  // Pack the structs to avoid destructive padding.
#pragma pack(push, 1)

/**
 * @brief Bitmap file header
 */
struct BitmapFileHeader
{
	/**
	 * @brief  Signature. Should be "BM"
	 */
	char bfType[2];

	/**
	 * @brief Size of the file in bytes.
	 */
	uint32_t bfSize;

	/**
	 * @brief Reserved bytes. Should be zero and not touched.
	 */
	uint32_t bfReserved;

	/**
	 * @brief Offset to start of pixel array.
	 */
	uint32_t bfOffBits;
};

/**
 * @brief The bitmap image header.
 */
struct BitmapImageHeader
{
	/**
	 * @brief Header size in bytes.
	 */
	uint32_t biSize;

	/**
	 * @brief Image width in pixels.
	 */
	uint32_t biWidth;

	/**
	 * @brief Image height in pixels.
	 */
	uint32_t biHeigth;

	/**
	 * @brief Number of image planes in file. Should be 1.
	 */
	uint16_t biPlanes;

	/**
	 * @brief Bits per pixel.
	 */
	uint32_t biBitCount;


	/**
	 * @brief  Compression type
	 * 0 - Uncompressed.
	 * 1 - RLE-8
	 * 2 - RLE-4
	 * 3 - Bitfields
	 */
	uint32_t biCompression;

	/**
	 * @brief Image size. Set to 0 if biCompression is 0. Then size is biWidth*biHeight.
	 */
	uint32_t biSizeImage;

	/**
	 * @brief Preferred resolution in pixels per meter.
	 */
	uint32_t biXPelsPerMeter;

	/**
	 * @brief Preferred resolution in pixels per meter.
	 */
	uint32_t biYPelsPerMeter;

	/**
	 * @brief Number of color map entries that are actually used.
	 */
	uint32_t biClrUsed;

	/**
	 * @brief Number of significant colors
	 */
	uint32_t biClrImportant;
};

// Unpack again.
#pragma pack(pop)

BMP::BMP(const char* filePath)
{
	// Open the binary file in a stream
	std::fstream hFile(filePath, std::ios::in | std::ios::binary);

	// Check if we got a valid file.
	if (!hFile.is_open())
	{
		throw std::invalid_argument(std::string("File (") + filePath + ") could not be opened.");
	}

	// Search for end and save position, then return to beginning.
	hFile.seekg(0, std::ios::end);
	std::size_t fileLength = hFile.tellg();
	hFile.seekg(0, std::ios::beg);

	// Read file header.
	BitmapFileHeader fileHeader;
	hFile.read(reinterpret_cast<char*>(&fileHeader), sizeof(BitmapFileHeader));

	// Read image header.
	BitmapImageHeader imageHeader;
	hFile.read(reinterpret_cast<char*>(&imageHeader), sizeof(BitmapImageHeader));

	// Check signature

	if (fileHeader.bfType[0] != 'B' || fileHeader.bfType[1] != 'M')
	{
		hFile.close();
		// Signature mismatch!
		throw std::invalid_argument(std::string("Signature mismatch in (") + filePath + "). Should be 'B''M', got '" + fileHeader.bfType[0] + "''" + fileHeader.bfType[1] + "'");
	}

	if (imageHeader.biBitCount != BI_BITCOUNT_24 && imageHeader.biBitCount != BI_BITCOUNT_32)
	{
		hFile.close();
		throw std::invalid_argument(std::string("Invalid file format in file (") + filePath + "). Expected 24 or 32 bit image.");
	}

	if (imageHeader.biCompression != BI_COMPRESSION_NONE)
	{
		hFile.close();
		throw std::invalid_argument(std::string("File (") + filePath + ") is compressed.");
	}

	// Extract image information

	// The bits per pixel.
	// Indicates RGB or RGBA pixel format.
	bitsPerPixel = imageHeader.biBitCount;

	// Width and height of image.
	width = imageHeader.biWidth;
	height = imageHeader.biHeigth;

	// Offset to pixel array from start of file.
	uint32_t pixelOffset = fileHeader.bfOffBits;

	// Size in bytes of pixel array.
	uint32_t size = (width * bitsPerPixel + 31) / 8 * height;

	// Resize the vector to the expected size.
	pixels.resize(size);

	// Set filestream marker at start of pixel array
	hFile.seekg(pixelOffset, std::ios::beg);

	// Read the array into the data region of the pixel vector
	hFile.read(reinterpret_cast<char*>(pixels.data()), size);

	// Close the filestream
	hFile.close();
}

const std::vector<uint8_t>& BMP::getPixels() const
{
	return pixels;
}

uint32_t BMP::getWidth() const
{
	return width;
}

uint32_t BMP::getHeight() const
{
	return height;
}

bool BMP::hasAlpha() const
{
	// If bits per pixel is 32, we have RGBA, otherwise we have RGB 
	return bitsPerPixel == 32;
}
