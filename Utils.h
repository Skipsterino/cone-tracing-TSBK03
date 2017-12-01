/**
 * @file	Utils.h
 * @Author	Joakim Bertils
 * @date	2017-02-08
 * @brief	TODO: Insert documentation
 */

#pragma once

#include <string>
#include <glm/glm.hpp>

/**
 * @brief Constructs a string using content of file at path
 * @param path File path relative to Solution
 * @return String with file contents
 */
std::string getStringFromFile(const std::string& path);

std::ostream& operator<<(std::ostream& os, const glm::vec3& vec);

std::istream& operator>>(std::istream& is, glm::vec3& vec);