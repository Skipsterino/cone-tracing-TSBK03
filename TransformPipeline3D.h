/**
 * @file	TransformPipeline3D.h
 * @Author	Joakim Bertils
 * @date	2017-03-07
 * @brief	3D Transformation pipeline to ease transform calculations.
 */

#pragma once

#include <glm/glm.hpp>

/**
 * @brief Class to represent a 3D transformation pipeline.
 */
class TransformPipeline3D
{
public:
	/**
	 * @brief Default constructor which sets all Matrices to Identity.
	 */
	TransformPipeline3D();

	/**
	 * \brief Sets the whole chain in a single call
	 * \param pos 
	 * \param angle 
	 * \param axis 
	 * \param scale 
	 */
	void setChain(glm::vec3 pos = glm::vec3(0.f), float angle = 0.f, glm::vec3 axis = glm::vec3(0.f, 1.f, 0.f), glm::vec3 scale = glm::vec3(1.f));

	/**
	 * @brief Applies a translatation to the model matrix.
	 * @param vec Translation parameters for each dimension.
	 */
	void translate(glm::vec3 vec);

	/**
	 * @brief Applies a rotation to the model matrix.
	 * @param angle Angle to rotate, in radians.
	 * @param axis Axis to rotate about.
	 */
	void rotate(float angle, glm::vec3 axis);
	
	/**
	 * @brief Applies a scaling to the model matrix.
	 * @param scale Scaling parameters for each dimension.
	 */
	void scale(glm::vec3 scale);

	/**
	 * @brief Sets a view matrix to be used.
	 * @param view New view matrix.
	 */
	void setView(glm::mat4 view);

	/**
	 * @brief Sets a new projection matrix.
	 * @param proj New projection matrix.
	 */
	void setProj(glm::mat4 proj);

	/**
	 * @brief Sets the parent transform for this transform.
	 * @param parent Ptr to parent transform.
	 */
	void setParentTransform(TransformPipeline3D* parent);

	/**
	 * @brief Returns the model transformation.
	 * @return Model matrix.
	 */
	glm::mat4 getModelTransform() const;

	/**
	 * @brief Returns the local transform.
	 * @return Local model matrix.
	 */
	glm::mat4 getLocalModelTransform() const;

	/**
	 * @brief Returns the full model-view-projection transformation.
	 * @return Projection Matrix * View Matrix * Model Matrix.
	 */
	glm::mat4 getMVP() const;
private:

	/**
	 * @brief Current model matrix.
	 */
	glm::mat4 model{ 1.f };
	
	/**
	 * @brief Current view matrix.
	 */
	glm::mat4 view{ 1.f };

	/**
	 * @brief Current projection matrix.
	 */
	glm::mat4 proj{ 1.f };
	
	/**
	 * @brief Ptr to parent transform object.
	 */
	TransformPipeline3D* parentTransform;
};