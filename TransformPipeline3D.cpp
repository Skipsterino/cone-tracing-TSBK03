/**
 * @file	TransformPipeline3D.cpp
 * @Author	Joakim Bertils
 * @date	2017-03-07
 * @brief	3D Transformation pipeline to ease transform calculations.
 */

#include "TransformPipeline3D.h"

#include <glm/gtc/matrix_transform.hpp>

TransformPipeline3D::TransformPipeline3D()
	: model{1.f}, view{1.f}, proj{1.f}
{
	parentTransform = nullptr;
}

void TransformPipeline3D::setChain(glm::vec3 pos, float angle, glm::vec3 axis, glm::vec3 scale)
{
	model = glm::translate(glm::rotate(glm::scale(glm::mat4(1.f), scale), angle, axis), pos);
	
}

void TransformPipeline3D::translate(glm::vec3 vec)
{
	model = glm::translate(model, vec);
}

void TransformPipeline3D::rotate(float angle, glm::vec3 axis)
{
	model = glm::rotate(model, angle, axis);
}

void TransformPipeline3D::scale(glm::vec3 scale)
{
	model = glm::scale(model, scale);
}

void TransformPipeline3D::setView(glm::mat4 view)
{
	this->view = view;
}

void TransformPipeline3D::setProj(glm::mat4 proj)
{
	this->proj = proj;
}

void TransformPipeline3D::setParentTransform(TransformPipeline3D* parent)
{
	parentTransform = parent;
}

glm::mat4 TransformPipeline3D::getModelTransform() const
{
	return model * ( parentTransform ? parentTransform->getModelTransform() : glm::mat4{1.f});
}

glm::mat4 TransformPipeline3D::getLocalModelTransform() const
{
	return model;
}

glm::mat4 TransformPipeline3D::getMVP() const
{
	return proj * view * model*( parentTransform ? parentTransform->getModelTransform() : glm::mat4{1.f});
}
