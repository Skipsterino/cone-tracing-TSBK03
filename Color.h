/**
 * @file	Color.h
 * @Author	Joakim Bertils
 * @date	2017-03-04
 * @brief	Class representing a color.
 */

#pragma once

#include <utility>

 /**
  * @brief Color class
  */
class Color
{
public:

	/**
	 * @brief Default constructor
	 *
	 * Creates a black color with full alpha value.
	 */
	Color() = default;

	/**
	 * @brief Grayscale Constructor
	 *
	 * Creates a grayscale color.
	 *
	 * @param grayScale Color strength.
	 * @param a Alpha channel value.
	 */
	explicit Color(float grayScale, float a = 1.f) : r{ grayScale }, g{ grayScale }, b{ grayScale }, a{ a } {}

	/**
	 * @brief Array Constructor
	 * @param array 4 element float array containing rgba data.
	 */
	explicit Color(float* array) : r{array[0]}, g{array[1]}, b{array[2]}, a{array[3]} {}

	/**
	 * @brief Full color Constructor
	 *
	 * Creates a color with specified rgba parameters.
	 *
	 * @param r Red channel value.
	 * @param g Green channel value.
	 * @param b Blue channel value.
	 * @param a Alpha channel value.
	 */
	Color(float r, float g, float b, float a = 1.f) : r{ r }, g{ g }, b{ b }, a{ a } {}

	/**
	 * @brief Copy constructor
	 * @param other Source object
	 */
	Color(const Color& other) : r{ other.r }, g{ other.g }, b{ other.b }, a{ other.a } {}

	/**
	 * @brief Move constructor
	 * @param other Source object
	 */
	Color(Color&& other) noexcept { swap(other); }

	/**
	 * @brief Copy Assignment Operator
	 * @param other Source object
	 * @return Reference to self.
	 */
	Color& operator=(const Color& other)
	{
		r = other.r;
		g = other.g;
		b = other.b;
		a = other.a;

		return *this;
	}

	/**
	* @brief Move Assignment Operator
	* @param other Source object
	* @return Reference to self.
	*/
	Color& operator=(Color&& other) noexcept
	{
		swap(other);
		return *this;
	}

	/**
	 * @brief Destructor
	 */
	~Color() {}

	/**
	 * @brief Swap member function
	 * @param other Source object
	 */
	void swap(Color& other) noexcept
	{
		std::swap(r, other.r);
		std::swap(g, other.g);
		std::swap(b, other.b);
		std::swap(a, other.a);
	}

	/**
	 * @brief Red channel value.
	 */
	float r{ 0.f };

	/**
	* @brief Green channel value.
	*/
	float g{ 0.f };

	/**
	* @brief Blue channel value.
	*/
	float b{ 0.f };

	/**
	* @brief Alpha channel value.
	*/
	float a{ 1.f };

	/**
	 * @brief Color swap function.
	 * @param first First color.
	 * @param second Second color.
	 */
	friend void swap(Color& first, Color& second) noexcept
	{
		std::swap(first.r, second.r);
		std::swap(first.g, second.g);
		std::swap(first.b, second.b);
		std::swap(first.a, second.a);
	}
};