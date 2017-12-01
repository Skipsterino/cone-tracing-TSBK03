/**
 * @file	PixelInfo.h
 * @Author	Joakim Bertils
 * @date	2017-02-09
 * @brief	PixelInfo
 */

#pragma once

#include <cstdint>

/**
 * @brief Union of the color and the 
 */
typedef union PixelInfo
{
	/**
	 * @brief The Color as a 32 bit value.
	 */
	uint32_t color;

	struct
	{
		/**
		 * @brief The Blue Channel
		 */
		uint8_t B;

		/**
		 * @brief The Green Channel
		 */
		uint8_t G;

		/**
		 * @brief The Red Channel 
		 */
		uint8_t R;

		/**
		 * @brief The Alpha Channel
		 */
		uint8_t A;
	};

} *PixelInfoPtr;