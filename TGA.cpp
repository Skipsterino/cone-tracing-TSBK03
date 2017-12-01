/**
 * @file	TGA.cpp
 * @Author	Joakim Bertils
 * @date	2017-05-21
 * @brief	TGA Texture file Implementation
 */

#include "TGA.h"

#include <fstream>
#include <iostream>

#define COLOR_MAP_NOT_INCLUDED			0
#define COLOR_MAP_INCLUDED				1

#define IS_TYPE_NO_DATA					0
#define IS_TYPE_UNCOMPRESSED_COLOR_MAP	1
#define IS_TYPE_UNCOMPRESSED_TRUE_COLOR	2
#define IS_TYPE_UNCOMPRESSED_BW			3
#define IS_TYPE_RUN_LENGTH_COLOR_MAP	9
#define IS_TYPE_RUN_LENGTH_TRUE_COLOR	10
#define IS_TYPE_RUN_LENGTH_BW			11

#define PD_BITCOUNT_1					1
#define PD_BITCOUNT_4					4
#define PD_BITCOUNT_8					8
#define PD_BITCOUNT_16					16
#define PD_BITCOUNT_24					24
#define PD_BITCOUNT_32					32

#pragma pack(push, 1)

// File Format
// 
//	---------------------
//  | File Header       |
//	---------------------
//  | Image ID          |
//	---------------------
//  | Color Map Data    |
//	---------------------
//  | Image Data        |
//	---------------------
//  | Developer Area    |
//	---------------------
//  | Extended Area     |
//	---------------------
//  | File Footer       |
//	---------------------

struct TGAColorMapSpecification
{
	uint16_t firstEntryIndex;
	uint16_t colorMapLength;
	uint8_t colorMapEntrySize;
};

struct TGAImageSpecification
{
	uint16_t xOrigin;
	uint16_t yOrigin;
	uint16_t imageWidth;
	uint16_t imageHeight;
	uint8_t pixelDepth;
	uint8_t imageDescriptor;
};

struct TGAFileHeader
{
	uint8_t IDLength;
	uint8_t colorMapType;
	uint8_t imageType;
	TGAColorMapSpecification colorMapSpecification;
	TGAImageSpecification imageSpecification;
};

struct TGADateTimeEntry
{
	uint16_t month;
	uint16_t day;
	uint16_t year;
	uint16_t hour;
	uint16_t minute;
	uint16_t second;
};

struct TGAJobTimeEntry
{
	uint16_t hours;
	uint16_t minutes;
	uint16_t seconds;
};

struct TGASoftwareVersion
{
	uint16_t versionNumber;
	char versionLetter;
};

struct TGAPixelAspectRatio
{
	uint16_t numerator;
	uint16_t denominator;
};

struct TGAGammaValue
{
	uint16_t numerator;
	uint16_t denominator;
};

struct TGAExtensionArea
{
	uint16_t extensionSize;
	char author[41];
	char authorComments[324];
	TGADateTimeEntry dateTimeEntry;
	char jobID[41];
	TGAJobTimeEntry jobTimeEntry;
	char softwareID[41];
	TGASoftwareVersion softwareVersion;
	uint32_t keyColor;
	TGAPixelAspectRatio aspectRatio;
	TGAGammaValue gammaValue;
	uint32_t colorCorrectionOffset;
	uint32_t postageStampOffset;
	uint32_t scanLineOffset;
	uint8_t attributesType;
};

struct TGAFileFooter
{
	uint32_t extensionAreaFooter;
	uint32_t developerDirectoryOffset;
	char signature[16];
	char reserved;
	char zeroTerminator;
};

#pragma pack(pop)

TGA::TGA(const char* filePath)
{
	std::fstream hFile(filePath, std::ios::in | std::ios::binary);

	if (!hFile.is_open())
	{
		throw std::invalid_argument(std::string("File (") + filePath + ") could not be opened.");
	}

	// Allocate header
	TGAFileHeader fileHeader;
	memset(&fileHeader, 0, sizeof(TGAFileHeader));

	// Read header into struct
	hFile.read(reinterpret_cast<char*>(&fileHeader), sizeof(TGAFileHeader));

	
	if(fileHeader.imageType == IS_TYPE_UNCOMPRESSED_TRUE_COLOR) // File is not compressed and in true color.
	{
		// Read file meta-data
		bitsPerPixel = fileHeader.imageSpecification.pixelDepth;
		width = fileHeader.imageSpecification.imageWidth;
		height = fileHeader.imageSpecification.imageHeight;
		size = (width * bitsPerPixel + 31) / 8 * height;

		// Check that it is the correct format.
		if(bitsPerPixel != PD_BITCOUNT_24 && bitsPerPixel != PD_BITCOUNT_32)
		{
			hFile.close();
			throw std::invalid_argument(std::string("Invalid file format in file (") + filePath + "). Expected 24 or 32 bit image.");
		}

		// Prepare pixel buffer.
		pixels.resize(size);
		isCompressed = false;

		// Read file content.
		hFile.read(reinterpret_cast<char*>(pixels.data()), size);
	} 
	else if(fileHeader.imageType == IS_TYPE_RUN_LENGTH_TRUE_COLOR) // File is compressed and in true color
	{
		// Read file meta-data
		bitsPerPixel = fileHeader.imageSpecification.pixelDepth;
		width = fileHeader.imageSpecification.imageWidth;
		height = fileHeader.imageSpecification.imageHeight;
		size = (width * bitsPerPixel + 31) / 8 * height;

		// Check that it is the correct format.
		if (bitsPerPixel != PD_BITCOUNT_24 && bitsPerPixel != PD_BITCOUNT_32)
		{
			hFile.close();
			throw std::invalid_argument(std::string("Invalid file format in file (") + filePath + "). Expected 24 or 32 bit image.");
		}

		// Allocate a pixel-struct.
		PixelInfo pixel;
		memset(&pixel, 0, sizeof(PixelInfo));

		int currentByte = 0;
		std::size_t currentPixel = 0;
		isCompressed = true;
		uint8_t chunkHeader = 0;
		int bytesPerPixel = (bitsPerPixel) / 8;
		pixels.resize(width * height * sizeof(PixelInfo));

		// Loop through each pixel in memory array.
		do
		{
			// Read chunk header
			hFile.read(reinterpret_cast<char*>(&chunkHeader), sizeof(chunkHeader));

			// If chunk header bit 7 is 0, add one to value and read that many pixels from file.
			if(chunkHeader < 128)
			{
				++chunkHeader;
				for (int i{ 0 }; i < chunkHeader; ++i, ++currentPixel)
				{
					hFile.read(reinterpret_cast<char*>(&pixel), bytesPerPixel);

					pixels[currentByte++] = pixel.B;
					pixels[currentByte++] = pixel.G;
					pixels[currentByte++] = pixel.R;

					// Take alpha channel into account if present.
					if(bitsPerPixel == PD_BITCOUNT_32)
					{
						pixels[currentByte++] = pixel.A;
					}
				}
			}
			// If chunk header bit 7 is 1, subtract 127 to header and read the next value.
			// Repeat that value chunk header times.
			else
			{
				chunkHeader -= 127;
				hFile.read(reinterpret_cast<char*>(&pixel), bytesPerPixel);

				for (int i{ 0 }; i < chunkHeader; ++i, ++currentPixel)
				{
					pixels[currentByte++] = pixel.B;
					pixels[currentByte++] = pixel.G;
					pixels[currentByte++] = pixel.R;

					// Take alpha channel into account if present.
					if (bitsPerPixel == PD_BITCOUNT_32)
					{
						pixels[currentByte++] = pixel.A;
					}
				}
			}
		} while (currentPixel < (height * width));
	}
	// Add more imageTypes here as needed.
	else
	{
		hFile.close();
		throw std::invalid_argument(std::string("Invalid file format in file (") + filePath + "). Expected compressed or uncompressed true-color format.");
	}

	hFile.close();
}

const std::vector<uint8_t>& TGA::getPixels() const
{
	return pixels;
}

uint32_t TGA::getWidth() const
{
	return width;
}

uint32_t TGA::getHeight() const
{
	return height;
}

bool TGA::hasAlpha() const
{
	return bitsPerPixel == PD_BITCOUNT_32;
}
