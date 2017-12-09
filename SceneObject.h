#pragma once

#include "TransformPipeline3D.h"
#include "RawModel.h"

class SceneObject
{
public:
	SceneObject(const char* path);
	~SceneObject();

	void draw();

	TransformPipeline3D* getTransform();

	void setChain(glm::vec3 pos = glm::vec3(0.f), float angle = 0.f, glm::vec3 axis = glm::vec3(0.f, 1.f, 0.f), glm::vec3 scale = glm::vec3(1.f));
	void translate(glm::vec3 vec);
	void rotate(float angle, glm::vec3 axis);
	void scale(glm::vec3 scale);
	void setView(glm::mat4 view);
	void setProj(glm::mat4 proj);
	void setParentTransform(TransformPipeline3D* parent);
	void setTexture(std::string str);
	std::string getTexture() const;
	glm::mat4 getModelTransform() const;
	glm::mat4 getLocalModelTransform() const;
	glm::mat4 getMVP() const;

	Material mat;
private:
	TransformPipeline3D tr;
	RawModel mo;
	std::string tex;

};

