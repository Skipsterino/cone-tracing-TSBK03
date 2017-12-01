/**
 * @file	Utils.cpp
 * @Author	Kevin Kjellén
 * @date	2017-11-01
 * @brief	Extended version of TSBK07 project Utils.cpp with Joakim Bertils
 */

#include "Utils.h"

#include <fstream>
#include <streambuf>

#include <glm/glm.hpp>

std::string getStringFromFile(const std::string& path)
{
	// Stream iterator magic
	return std::string{ std::istreambuf_iterator<char>(std::ifstream(path)), std::istreambuf_iterator<char>() };
}

std::ostream& operator<<(std::ostream& os,const glm::vec3& vec)
{
	return os << '{' << vec.x << ',' << vec.y << ',' << vec.z << '}';
}

std::istream& operator >> (std::istream& is, glm::vec3& vec)
{
	is >> vec.x >> vec.y >> vec.z;

	return is;
}
