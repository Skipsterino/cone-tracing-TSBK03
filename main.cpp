
#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.h"
#include "ShaderProgram.h"
#include "loadobj.h"
#include "Camera.h"
#include "TGA.h"
#include "Texture2D.h"

#include "Utils.h"

#include "RawModel.h"
#include "SceneObject.h"
#include "CornellScene.h"


void GLFWError(int errorCode, const char* message)
{
	std::cerr << "Recieved error " << errorCode << ":" << message << std::endl;
}

int main(int argc, char **argv)
{

try
{

	WindowSettings settings = getDefaultWindowSettings();
	//settings.maximized = true;
	Window window{ 1080, 1080, "Hue", settings };

	glfwSetErrorCallback(GLFWError);

	//============================================================================
	// Final Setup
	//============================================================================

	GLfloat time = (GLfloat)glfwGetTime();
	GLfloat timeElapsed = 0.f;
	GLuint frames = 0;

	CornellScene cornell{ &window };

	while (!window.shouldClose())
	{
		// Recalculate time and FPS counter
		GLfloat oldTime = time;
		time = (GLfloat)glfwGetTime();
		GLfloat timeDelta = (time - oldTime);
		timeElapsed += timeDelta;
		++frames;
		if (timeElapsed > 1.f)
		{
			timeElapsed -= 1.f;
			std::string newTitle = std::to_string(frames) + std::string{ " FPS" };
			window.setTitle(newTitle);
			frames = 0;
		}

		// Handle window events
		WindowEvent ev;
		while (window.pollEvent(ev))
		{
			cornell.handleEvent(ev, timeDelta);
		}

		cornell.drawScene();

		cornell.update(timeDelta, time);

		window.display();
	}

	glfwTerminate();
}
catch (const std::exception& ex)
{
	std::cerr << "[FATAL] Caught exception at main level:" << std::endl << ex.what() << std::endl;
}
catch (...)
{
	std::cerr << "[FATAL] Caught unknown exception at main level" << std::endl;
}

return 0;

}