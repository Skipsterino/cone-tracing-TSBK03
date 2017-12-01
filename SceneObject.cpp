/**
* @file	SceneObject.cpp
* @Author	Kevin Kjellén
* @date	2017-11-01
* @brief	Class holding a model and a transform
*/

#include "SceneObject.h"

SceneObject::SceneObject(const char* path) : tr{}, mo{path}
{
}


SceneObject::~SceneObject()
{
}

void SceneObject::draw()
{
	mo.draw();
}

TransformPipeline3D* SceneObject::getTransform()
{
	return &tr;
}

void SceneObject::setChain(glm::vec3 pos, float angle, glm::vec3 axis, glm::vec3 scale)
{
	tr.setChain(pos, angle, axis, scale);
}

void SceneObject::translate(glm::vec3 vec)
{
	tr.translate(vec);
}
void SceneObject::rotate(float angle, glm::vec3 axis)
{
	tr.rotate(angle, axis);
}
void SceneObject::scale(glm::vec3 scale)
{
	tr.scale(scale);
}
void SceneObject::setView(glm::mat4 view)
{
	tr.setView(view);
}
void SceneObject::setProj(glm::mat4 proj)
{
	tr.setProj(proj);
}
void SceneObject::setParentTransform(TransformPipeline3D* parent)
{
	tr.setParentTransform(parent);
}
glm::mat4 SceneObject::getModelTransform() const
{
	return tr.getModelTransform();
}
glm::mat4 SceneObject::getLocalModelTransform() const
{
	return tr.getLocalModelTransform();
}
glm::mat4 SceneObject::getMVP() const
{
	return tr.getMVP();
}
