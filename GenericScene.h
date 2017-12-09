/**
* @file	GenericScene.h
* @Author	Kevin Kjellén
* @date	2017-11-01
* @brief	Abstract class for a generic scene object
*/

#pragma once
#include <vector>
#include "SceneObject.h"
#include "Camera.h"
#include "Window.h"
#include "Texture2D.h"
#include "Texture3D.h"

typedef std::map<std::string, Texture2D*> TextureMap;
typedef std::map<std::string, ShaderProgram*> ShaderMap;
typedef std::map<std::string, SceneObject*> ObjMap;

class GenericScene
{
public:
	GenericScene() = default;
	virtual ~GenericScene();

	virtual void update(GLfloat timedelta, GLfloat timeElapsed) = 0;
	virtual void drawScene() = 0;
	virtual void handleEvent(WindowEvent& ev, GLfloat timedelta) = 0;
protected:
	ObjMap sceneObjs{};
	ShaderMap shaders{};
	TextureMap textures{};
	Texture3D* voxelization{};
	Camera cam{};
	Material mat{ glm::vec3(0,0,0), glm::vec3(0,0,0), glm::vec3(0,0,0), 0};
	PointLight light{ glm::vec3(0,0,0), glm::vec3(0,0,0), glm::vec3(0,0,0), glm::vec3(0,0,0) };
	bool forwardKeyPressed{ false };
	bool backwardKeyPressed{ false };
	bool leftKeyPressed{ false };
	bool rightKeyPressed{ false };
	bool spaceKeyPressed{ false };
	bool firstMouse{ true };
	GLfloat lastX{ 0 };
	GLfloat lastY{ 0 };
	Window* windowPtr{nullptr};
	glm::mat4 projMat{};
	glm::mat4 orthMat{};
};

