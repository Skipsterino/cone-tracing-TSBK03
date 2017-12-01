/**
 * @file	Texture2D.h
 * @Author	Joakim Bertils
 * @date	2017-03-05
 * @brief	2-Dimensional texture wrapper class.
 *
 * https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexParameter.xhtml
 * https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glTexImage2D.xhtml
 * https://www.youtube.com/watch?v=dQw4w9WgXcQ
 */

#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "TextureFile.h"

#include <map>

#include "Color.h"
#include "EngineDLL.h"

 // Texture1D
 // Texture2D
 // Texture3D
 // TextureCubeMap
 // Texture1DArray
 // Texture2DArray
 // Texture3DArray
 // TextureCubeMapArray

 /**
  * @brief Texture Mode.
  *
  * Specifies the mode used to read from depth-stencil format textures.
  * The initial value is DEPTH_COMPONENT.
  */
enum class TEXTURE_2D_TEXTURE_MODE
{
	/**
	 * @brief Stencil Index Mode.
	 *
	 * If the depth stencil mode is STENCIL_INDEX then the stencil
	 * component is returned in Rt and the depth component is discarded.
	 */
	STENCIL_INDEX,

	/**
	 * @brief Depth Component Mode.
	 *
	 * If the depth stencil mode is DEPTH_COMPONENT, then reads from
	 * depth-stencil format textures will return the depth component
	 * of the texel in Rt and the stencil component will be discarded.
	 */
	DEPTH_COMPONENT,

	/**
	 * @brief Invalid value.
	 */
	INVALID_VALUE
};

/**
 * @brief Texture 2D Compare Function
 *
 * Specifies the comparison operator used when TEXTURE_2D_COMPARE_MODE
 * is set to COMPARE_REF_TO_TEXTURE.
 */
enum class TEXTURE_2D_COMPARE_FUNC
{
	/**
	 * @brief Less or equal operator.
	 */
	LESS_EQUAL,

	/**
	 * @brief Greater or equal operator.
	 */
	GREATER_EQUAL,

	/**
	 * @brief Less operator.
	 */
	LESS,

	/**
	 * @brief Greater operator.
	 */
	GREATER,

	/**
	 * @brief Equal operator.
	 */
	EQUAL,

	/**
	 * @brief Not equal operator.
	 */
	NOT_EQUAL,

	/**
	 * @brief Always pass test.
	 */
	ALWAYS,

	/**
	 * @brief Never pass test.
	 */
	NEVER,

	/**
	 * @brief Invalid value.
	 */
	INVALID_VALUE
};

/**
 * @brief Texture 2D compare mode.
 *
 * Specifies the texture comparison mode for currently bound depth textures.
 */
enum class TEXTURE_2D_COMPARE_MODE
{
	/**
	 * @brief Compare Ref to Texture.
	 *
	 * Specifies that the interpolated and clamped r texture coordinate should
	 * be compared to the value in the currently bound depth texture. The result
	 * of the comparison is assigned to the red channel.
	 */
	COMPARE_REF_TO_TEXTURE,

	/**
	 * @brief No comparison.
	 *
	 * Specifies that the red channel should be assigned the appropriate value
	 * from the currently bound depth texture.
	 */
	NONE,

	/**
	 * @brief Invalid value.
	 */
	INVALID_VALUE
};

/**
 * @brief Texture 2D color swizzling components. 
 */
enum class TEXTURE_2D_SWIZZLE_COMPONENT
{
	/**
	 * @brief Red channel
	 */
	RED,
	
	/**
	 * @brief Green channel
	 */
	GREEN,
	
	/**
	 * @brief Blue channel
	 */
	BLUE,
	
	/**
	 * @brief Alpha channel
	 */
	ALPHA,
	
	/**
	 * @brief Always zero.
	 */
	ZERO,
	
	/**
	 * @brief Always one.
	 */
	ONE,
	
	/**
	 * @brief Invalide value.
	 */
	INVALID_VALUE
};

/**
 * @brief Texture Wrap Mode
 *
 * Specifies behavouir when coordinates are outside texture bounds.
 */
enum class TEXTURE_2D_WRAP
{
	/**
	 * @brief Repeat
	 *
	 * Texture will be repeated.
	 */
	REPEAT,

	/**
	 * @brief Mirrored Repeat
	 *
	 * Texture will be repeated but mirrored in reference to the adjacent
	 * entry.
	 */
	MIRRORED_REPEAT,

	/**
	 * @brief Clamp to Edge.
	 *
	 * Color will be set to the nearest edge color.
	 */
	CLAMP_TO_EDGE,

	/**
	 * @brief Clamp to border.
	 *
	 * Color will be set to the predefined border color set with
	 * setTextureBorderColor().
	 */
	CLAMP_TO_BORDER,

	/**
	 * @brief Invalid value.
	 */
	INVALID_VALUE
};

/**
 * @brief Texture 2D Filter Mode.
 *
 * Specifies behaviour when minifying or magnifying textures.
 */
enum class TEXTURE_2D_FILTERING
{
	/**
	 * @brief Nearest Texel.
	 *
	 * Returns the value of the texture element that is nearest (in Manhattan
	 * distance) to the specified texture coordinates.
	 */
	NEAREST,

	/**
	 * @brief Linear Texel Interpolation.
	 *
	 * Returns the weighted average of the four texture elements that are
	 * closest to the specified texture coordinates.
	 */
	LINEAR,

	/**
	 * @brief Nearest Mipmap Nearest Texel.
	 *
	 * Chooses the mipmap that most closely matches the size of the pixel
	 * being textured and uses the NEAREST criterion (the texture element
	 * closest to the specified texture coordinates) to produce a texture value.
	 */
	NEAREST_MIPMAP_NEAREST,

	/**
	 * @brief Nearest Mipmap Linear Texel Interpolation.
	 *
	 * Chooses the mipmap that most closely matches the size of the pixel being
	 * textured and uses the LINEAR criterion (a weighted average of the
	 * four texture elements that are closest to the specified texture
	 * coordinates) to produce a texture value.
	 */
	LINEAR_MIPMAP_NEAREST,

	/**
	 * @brief Linear Mipmap Nearest Texel.
	 *
	 * Chooses the two mipmaps that most closely match the size of the pixel
	 * being textured and uses the NEAREST criterion (the texture element
	 * closest to the specified texture coordinates) to produce a texture
	 * value from each mipmap. The final texture value is a weighted average
	 * of those two values.
	 */
	NEAREST_MIPMAP_LINEAR,

	/**
	 * @brief Linear Mipmap Linear Texel Interpolation.
	 *
	 * Chooses the two mipmaps that most closely match the size of the pixel
	 * being textured and uses the LINEAR criterion (a weighted average of
	 * the texture elements that are closest to the specified texture
	 * coordinates) to produce a texture value from each mipmap. The final
	 * texture value is a weighted average of those two values.
	 */
	LINEAR_MIPMAP_LINEAR,

	/**
	 * @brief Invalid Value.
	 */
	INVALID_VALUE
};

// Fyll på efter behov.
enum class TEXTURE_2D_FORMAT
{
	DEPTH_COMPONENT,
	RGB,
	RED,
	INVALID_VALUE
};

// Fyll på efter behov.
enum class TEXTURE_2D_DATATYPE
{
	FLOAT,
	UNSIGNED_BYTE,
	INVALID_VALUE
};

/**
 * @brief Class Representing a 2D Texture
 */
class ENGINE_API Texture2D
{
public:

	/**
	 * @brief Default constructor.
	 *
	 * Creates an empty object.
	 */
	Texture2D();

	/**
	 * @brief Constructor
	 * 
	 * Creates a texture from file. 
	 * 
	 * @param filePath Path to texture file.
	 * @param sWrap Wrapping behaviour in S-direction.
	 * @param tWrap Wrapping behaviour in T-direction.
	 * @param magFilter Magnifying filter behaviour.
	 * @param minFilter Minifying filter behaviour.
	 */
	explicit Texture2D(
		const char* filePath,
		TEXTURE_2D_WRAP sWrap = TEXTURE_2D_WRAP::REPEAT,
		TEXTURE_2D_WRAP tWrap = TEXTURE_2D_WRAP::REPEAT,
		TEXTURE_2D_FILTERING magFilter = TEXTURE_2D_FILTERING::LINEAR,
		TEXTURE_2D_FILTERING minFilter = TEXTURE_2D_FILTERING::LINEAR_MIPMAP_LINEAR
	);

	/**
	 * @brief Constructor
	 * 
	 * Generates an potentially empty texture
	 * 
	 * @param width Width, in pixels.
	 * @param height Height, in pixels
	 * @param format Format of the data. 
	 * @param type Datatype.
	 */
	Texture2D(
		GLuint width,
		GLuint height,
		TEXTURE_2D_FORMAT format,
		TEXTURE_2D_DATATYPE type,
		GLvoid* data = NULL
	);

	/**
	 * @brief Destructor.
	 */
	~Texture2D();

	/**
	 * @brief Copy constructor.
	 * @note Deleted.
	 */
	Texture2D(const Texture2D&) = delete;

	/**
	 * @brief Copy assignment operator.
	 * @return Ref to self.
	 * @note Deleted.
	 */
	Texture2D& operator=(const Texture2D&) = delete;

	/**
	 * @brief Move constructor
	 * @param other Source object
	 */
	Texture2D(Texture2D&& other) noexcept;

	/**
	 * @brief Returns the width of the source image.
	 * @return Width.
	 */
	GLuint getWidth() const;

	/**
	* @brief Returns the height of the source image.
	* @return Height.
	*/
	GLuint getHeight() const;

	/**
	 * @brief Returns the openGL texture handle.
	 * @return Texture Handle.
	 */
	GLuint getHandle() const;

	/**
	 * @brief Depth-stencil format texture read mode.
	 *
	 * Specifies the mode used to read from depth-stencil format textures.
	 *
	 * @param mode Desired mode.
	 */
	void setDepthStencilTextureMode(TEXTURE_2D_TEXTURE_MODE mode);

	/**
	 * @brief Texture border color.
	 *
	 * Defines the color to be sampled outside the texture if wrap mode is
	 * set to CLAMP_TO_BORDER.
	 *
	 * @param color Border Color.
	 */
	void setTextureBorderColor(Color color);

	/**
	 * @brief Texture Base Level
	 *
	 * Specifies the index of the lowest defined mipmap level.
	 *
	 * @param baseLevel Index.
	 */
	void setTextureBaseLevel(GLint baseLevel);

	/**
	* @brief Texture Max Level
	*
	* Specifies the index of the highest defined mipmap level.
	*
	* @param maxLevel Index.
	*/
	void setTextureMaxLevel(GLint maxLevel);

	/**
	 * @brief Sets the texture compare function.
	 * @param func Function to set.
	 */
	void setTextureCompareFunc(TEXTURE_2D_COMPARE_FUNC func);

	/**
	 * @brief Sets the texture compare mode.
	 * @param mode Mode to set.
	 */
	void setTextureCompareMode(TEXTURE_2D_COMPARE_MODE mode);

	/**
	 * @brief Sets the texture Level of Detail bias.
	 * @param LODBias New value for bias.
	 */
	void setTextureLODBias(GLint LODBias);

	/**
	 * @brief Sets minimum Level of Detail value.
	 * @param minLOD New value for minimum LOD.
	 */
	void setTextureMinLOD(GLint minLOD);

	/**
	* @brief Sets maximum Level of Detail value.
	* @param maxLOD New value for maximum LOD.
	*/
	void setTextureMaxLOD(GLint maxLOD);

	/**
	 * @brief Sets the s-coordinate wrap behaviour.
	 * @param sWrap New behaviour.
	 */
	void setTextureWrapS(TEXTURE_2D_WRAP sWrap);

	/**
	* @brief Sets the t-coordinate wrap behaviour.
	* @param tWrap New behaviour.
	*/
	void setTextureWrapT(TEXTURE_2D_WRAP tWrap);

	/**
	* @brief Sets the r-coordinate wrap behaviour.
	* @param rWrap New behaviour.
	*/
	void setTextureWrapR(TEXTURE_2D_WRAP rWrap);

	/**
	 * @brief Set magnifying filter behavour.
	 * @param magFilter New behaviour.
	 */
	void setTextureMagFilter(TEXTURE_2D_FILTERING magFilter);

	/**
	* @brief Set minifying filter behavour.
	* @param minFilter New behaviour.
	*/
	void setTextureMinFilter(TEXTURE_2D_FILTERING minFilter);

	/**
	 * @brief Set swizzle component for first channel. 
	 * @param comp Component to use.
	 */
	void setTextureSwizzleR(TEXTURE_2D_SWIZZLE_COMPONENT comp);

	/**
	* @brief Set swizzle component for second channel.
	* @param comp Component to use.
	*/
	void setTextureSwizzleG(TEXTURE_2D_SWIZZLE_COMPONENT comp);

	/**
	* @brief Set swizzle component for third channel.
	* @param comp Component to use.
	*/
	void setTextureSwizzleB(TEXTURE_2D_SWIZZLE_COMPONENT comp);

	/**
	* @brief Set swizzle component for fourth channel.
	* @param comp Component to use.
	*/
	void setTextureSwizzleA(TEXTURE_2D_SWIZZLE_COMPONENT comp);

	/**
	* @brief Gets Depth-stencil format texture read mode.
	* @return Current Mode.
	*/
	TEXTURE_2D_TEXTURE_MODE getDepthStencilTextureMode() const;

	/**
	 * @brief Gets border color.
	 * @return Current Color.
	 */
	Color getTextureBorderColor() const;

	/**
	 * @brief Gets mipmap base level.
	 * @return Current base level.
	 */
	GLint getTextureBaseLevel() const;

	/**
	 * @brief Gets mipmap max level.
	 * @return Current max level.
	 */
	GLint getTextureMaxLevel() const;

	TEXTURE_2D_COMPARE_FUNC getTextureCompareFunc() const;

	TEXTURE_2D_COMPARE_MODE getTextureCompareMode() const;

	GLint getTextureLODBias() const;

	GLint getTextureMinLOD() const;

	GLint getTextureMaxLOD() const;

	TEXTURE_2D_WRAP getTextureWrapS() const;

	TEXTURE_2D_WRAP getTextureWrapT() const;

	TEXTURE_2D_WRAP getTextureWrapR() const;

	TEXTURE_2D_FILTERING getTextureMagFilter() const;

	TEXTURE_2D_FILTERING getTextureMinFilter() const;

	TEXTURE_2D_SWIZZLE_COMPONENT getTextureSwizzleR() const;

	TEXTURE_2D_SWIZZLE_COMPONENT getTextureSwizzleG() const;
	
	TEXTURE_2D_SWIZZLE_COMPONENT getTextureSwizzleB() const;
	
	TEXTURE_2D_SWIZZLE_COMPONENT getTextureSwizzleA() const;
	
	/**
	 * @brief Bind texture to current unit.
	 */
	void bind() const;

	
	/**
	 * @brief Bind texture to specified unit.
	 * @param texUnit Unit to bind to.
	 */
	void bind(GLuint texUnit) const;

private:

	/**
	 * @brief Width of texture.
	 */
	GLuint width;
	
	/**
	 * @brief Height of texture.
	 */
	GLuint height;

	/**
	 * @brief OpenGL texture handle.
	 */
	GLuint textureID;

	/**
	 * @brief Swaps two textures.
	 * @param first First texture.
	 * @param second Second texture.
	 */
	friend void swap(Texture2D& first, Texture2D& second) noexcept;
};

typedef std::map<std::string, Texture2D*> Texture2DMap;