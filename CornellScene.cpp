/**
* @file	CornellScene.cpp
* @Author	Kevin Kjellén
* @date	2017-11-01
* @brief	Abstract class for a generic scene object
*/

#include "CornellScene.h"
#include <iostream>


CornellScene::CornellScene(Window* window) : GenericScene(), voxelGridSize{64}, voxelGrid{nullptr}, cycleMode{0}
{
	windowPtr = window;
	windowPtr->setCursorMode(CursorMode::DISABLED);

	projMat = glm::perspective(glm::radians(45.f), (GLfloat)windowPtr->getDimensions().x / (GLfloat)windowPtr->getDimensions().y, 0.1f, 100.f);
	orthMat = glm::ortho(-1, 1, -1, 1, -1, 1);
	// openGL setup
	glClearColor(0.f, 0.f, 0.f, 1.0f);
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_CONSERVATIVE_RASTERIZATION_NV);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Set render wireframe mode
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Light and material setup
	{
		// TODO: REMOVE GLOBAL MATERIAL
		mat.setAmbient(glm::vec3(1.f, 1.f, 1.f));
		mat.setDiffuse(glm::vec3(1.f, 1.f, 1.f));
		mat.setSpecular(glm::vec3(0.5f, 0.5f, 0.5f));
		mat.setShininess(0.6f*128.f);
		mat.setEmissivity(0.0f);

		light.setPosition(glm::vec3(0.f, 0.85f, 0.f));
		light.setAmbient(glm::vec3(1.f, 1.f, 1.f) * 0.5f);
		light.setDiffuse(glm::vec3(0.7f, 0.7f, 0.7f));
		light.setSpecular(glm::vec3(0.3f, 0.3f, 0.3f));
		light.setConstant(1.f);
		light.setLinear(0.f);
		light.setQuadratic(1.f);
	}

    // Object init
	{
		SceneObject* box = new SceneObject{ "resc/cornellTextCoords.obj" };
		box->rotate(glm::radians(-90.f), glm::vec3(0.f, 1.f, 0.f));
		box->scale(glm::vec3(0.999f, 0.999f, 0.999f));
		box->mat.setAmbient(glm::vec3(1.f, 1.f, 1.f));
		box->mat.setDiffuse(glm::vec3(1.f, 1.f, 1.f));
		box->mat.setSpecular(glm::vec3(0.5f, 0.5f, 0.5f));
		box->mat.setShininess(0.6f*128.f);
		box->mat.setEmissivity(0.0f);
		box->mat.setDiffuseReflectivity(1.f);
		box->mat.setSpecularReflectivity(0.1f);
		box->setTexture("Cornell");
		sceneObjs.emplace("Box", box);

		SceneObject* bunny = new SceneObject{ "resc/bunnyHD.obj" };
		bunny->translate(glm::vec3(0.36f, 0.0f, -0.38f));
		bunny->scale(glm::vec3(0.3f, 0.3f, 0.3f));
		bunny->mat.setAmbient(glm::vec3(1.f, 1.f, 1.f));
		bunny->mat.setDiffuse(glm::vec3(1.f, 1.f, 1.f));
		bunny->mat.setSpecular(glm::vec3(0.5f, 0.5f, 0.5f));
		bunny->mat.setShininess(0.6f*128.f);
		bunny->mat.setEmissivity(0.7f);
		bunny->mat.setDiffuseReflectivity(1.f);
		bunny->mat.setSpecularReflectivity(0.1f);
		bunny->setTexture("Cornell");
		sceneObjs.emplace("Bunny", bunny);

		SceneObject* teapot = new SceneObject{ "resc/teapot.obj" };
		teapot->rotate(glm::radians(90.f), glm::vec3(-1, 0, 0));
		teapot->translate(glm::vec3(-0.23f, -0.51f, -0.56f));
		teapot->scale(glm::vec3(0.1f, 0.1f, 0.1f));
		teapot->mat.setAmbient(glm::vec3(1.f, 1.f, 1.f));
		teapot->mat.setDiffuse(glm::vec3(1.f, 1.f, 1.f));
		teapot->mat.setSpecular(glm::vec3(0.5f, 0.5f, 0.5f));
		teapot->mat.setShininess(0.6f*128.f);
		teapot->mat.setEmissivity(0.0f);
		teapot->mat.setDiffuseReflectivity(1.f);
		teapot->mat.setSpecularReflectivity(1.f);
		teapot->setTexture("Concrete");
		sceneObjs.emplace("Teapot", teapot);

		SceneObject* ball = new SceneObject{ "resc/ball.obj" };
		ball->mat.setAmbient(glm::vec3(1.f, 1.f, 1.f));
		ball->mat.setDiffuse(glm::vec3(1.f, 1.f, 1.f));
		ball->mat.setSpecular(glm::vec3(0.5f, 0.5f, 0.5f));
		ball->mat.setShininess(0.6f*128.f);
		ball->mat.setEmissivity(1.f);
		ball->setTexture("Cornell");
		sceneObjs.emplace("Ball", ball);

	}

	// Shader init
	ShaderProgram* shaderProgram = new ShaderProgram{ "resc/shaders/coneTracingVert.shader", "resc/shaders/coneTracingFrag.shader" };
	try
	{
		shaderProgram->compile();
		shaderProgram->bindAttribLocation(0, "vertex_position");
		shaderProgram->bindAttribLocation(1, "vertex_normal");
		shaderProgram->bindAttribLocation(2, "vertex_texture_coordinates");
		shaderProgram->link();
	}
	catch (const ShaderProgramException& ex)
	{
		std::cerr << ex.what() << std::endl;
		glfwTerminate();
	}
	shaders.emplace("ConeTracing" , shaderProgram);
	
	shaderProgram = new ShaderProgram{ "resc/shaders/voxelizationVert.shader", "resc/shaders/voxelizationFrag.shader", "resc/shaders/voxelizationGeom.shader" };
	try
	{
		shaderProgram->compile();
		shaderProgram->bindAttribLocation(0, "vertex_position");
		shaderProgram->bindAttribLocation(1, "vertex_normal");
		shaderProgram->bindAttribLocation(2, "vertex_texture_coordinates");
		shaderProgram->link();
	}
	catch (const ShaderProgramException& ex)
	{
		std::cerr << ex.what() << std::endl;
		glfwTerminate();
	}
	shaders.emplace("Voxelization", shaderProgram);
	
	// Texture init
	textures.emplace("Concrete", new Texture2D{ "resc/conc.tga" });
	textures.emplace("Flower", new Texture2D{ "resc/maskros512.tga" });
	textures.emplace("Cornell", new Texture2D{ "resc/cornellUVtextureRasp.tga" });

	const std::vector<GLfloat> voxelGridData(4 * voxelGridSize * voxelGridSize * voxelGridSize, 0.0f);
	voxelGrid = new Texture3D(voxelGridData, voxelGridSize, voxelGridSize, voxelGridSize);

	cam.setPosition(glm::vec3(-3.3f, 0.f, 0.f));
}


CornellScene::~CornellScene()
{
	delete voxelGrid;
}

void CornellScene::update(GLfloat timeDelta, GLfloat timeElapsed)
{

	light.setPosition(glm::vec3(0.5f * sin(timeElapsed), 0.5f, 0.5f * cos(timeElapsed)));
	sceneObjs.at("Ball")->setChain(glm::vec3(20.f * sin(timeElapsed), 20.f, 20.f * cos(timeElapsed)), 0.f, glm::vec3(1.f), 0.025f * glm::vec3(1.f));
	//cam.setPosition(glm::vec3(0.6f * sin(timeElapsed), 0.5f, 0.6f * cos(timeElapsed)));

	if (forwardKeyPressed)
		cam.processKeyboard(FORWARD, timeDelta);
	if (backwardKeyPressed)
		cam.processKeyboard(BACKWARD, timeDelta);
	if (leftKeyPressed)
		cam.processKeyboard(LEFT, timeDelta);
	if (rightKeyPressed)
		cam.processKeyboard(RIGHT, timeDelta);
	if (spaceKeyPressed)
		cam.processKeyboard(ABSUP, timeDelta);

	cam.update();
}

void CornellScene::drawScene()
{
	// Clear screen and depth buffer
	GLfloat clearColor[4] = { 0, 0, 0, 0 };
	voxelGrid->Clear(clearColor);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Voxelize	
	glViewport(0, 0, voxelGridSize, voxelGridSize);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	ShaderProgram* shader = shaders.at("Voxelization");
	shader->use();
	for (auto i : sceneObjs)
	{
		voxelGrid->bind(0);
		shader->uploadUniform("voxGrid", 0);
		glBindImageTexture(0, voxelGrid->textureID, 0, GL_TRUE, 0, GL_WRITE_ONLY, GL_RGBA8);

		i.second->setView(glm::lookAt(glm::vec3(-1.f, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));
		i.second->setProj(orthMat);
		shader->uploadUniform("transform", i.second->getMVP());
		shader->uploadUniform("model", i.second->getModelTransform());
		shader->uploadUniform("view_pos", cam.getPosition());

		shader->uploadUniform("light", light);
		shader->uploadUniform("material", i.second->mat);

		shader->uploadUniform("texUnit", 1);
		textures.at(i.second->getTexture())->bind(1);

		i.second->draw();
	}
	glBindTexture(GL_TEXTURE_3D, voxelGrid->textureID);
	glGenerateMipmap(GL_TEXTURE_3D);
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		std::cerr << "OpenGL error: " << err << std::endl;
	}
	
	// Cone tracing
	glViewport(0, 0, windowPtr->getWidth(), windowPtr->getHeight());
	glClearColor(0.f, 0.f, 0.f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	shader = shaders.at("ConeTracing");
	shader->use();
	for(auto i : sceneObjs)
	{
		i.second->setView(cam.getViewMatrix());
		i.second->setProj(projMat);
		shader->uploadUniform("transform", i.second->getMVP());
		shader->uploadUniform("model", i.second->getModelTransform());
		shader->uploadUniform("view_pos", cam.getPosition());

		shader->uploadUniform("light", light);
		shader->uploadUniform("material", i.second->mat);

		shader->uploadUniform("gridSize", voxelGridSize);
		shader->uploadUniform("Mode", cycleMode);

		shader->uploadUniform("texUnit", 1);
		textures.at(i.second->getTexture())->bind(1);

		voxelGrid->bind(0);
		shader->uploadUniform("voxGrid", 0);

		i.second->draw();
	}
}

void CornellScene::handleEvent(WindowEvent& ev,  GLfloat timedelta)
{
	switch (ev.type)
	{
	case EventType::KEY_EVENT:
		if (ev.key.key == GLFW_KEY_W)
		{
			if (ev.key.action == Action::PRESS)
				forwardKeyPressed = true;
			else if (ev.key.action == Action::RELEASE)
				forwardKeyPressed = false;
		}
		else if (ev.key.key == GLFW_KEY_S)
		{
			if (ev.key.action == Action::PRESS)
				backwardKeyPressed = true;
			else if (ev.key.action == Action::RELEASE)
				backwardKeyPressed = false;
		}
		else if (ev.key.key == GLFW_KEY_A)
		{
			if (ev.key.action == Action::PRESS)
				leftKeyPressed = true;
			else if (ev.key.action == Action::RELEASE)
				leftKeyPressed = false;
		}
		else if (ev.key.key == GLFW_KEY_D)
		{
			if (ev.key.action == Action::PRESS)
				rightKeyPressed = true;
			else  if (ev.key.action == Action::RELEASE)
				rightKeyPressed = false;
		}
		else if (ev.key.key == GLFW_KEY_SPACE)
		{
			if (ev.key.action == Action::PRESS)
				spaceKeyPressed = true;
			else  if (ev.key.action == Action::RELEASE)
				spaceKeyPressed = false;
		}
		else if (ev.key.key == GLFW_KEY_C)
		{
			if (ev.key.action == Action::RELEASE)
				cycleMode = (cycleMode + 1) % 6;
		}
		else if (ev.key.key == GLFW_KEY_P)
		{
			if (ev.key.action == Action::PRESS)
				cam.tempPrintCoords();
		}
		break;
	case EventType::MOUSE_MOVED_EVENT:
	{
		if (firstMouse)
		{
			lastX = ev.mouse.posx;
			lastY = ev.mouse.posy;
			firstMouse = false;
		}

		GLfloat xOffset = ev.mouse.posx - lastX;
		GLfloat yOffset = lastY - ev.mouse.posy;

		lastX = ev.mouse.posx;
		lastY = ev.mouse.posy;

		cam.processMouseMovement(xOffset, yOffset);
	}
	break;
	default:
		break;
	}
}
