/**
 * @file	Window.cpp
 * @Author	Joakim Bertils
 * @date	2017-02-08
 * @brief	Window Class implementation
 */

#include "Window.h"

int Window::INSTANCE_COUNT = 0;

void framebuffer_size_callback_func(GLFWwindow* windowHandle, int width, int height)
{
	static_cast<Window*>(glfwGetWindowUserPointer(windowHandle))->framebuffer_size_callback(width, height);
}

void key_callback_func(GLFWwindow* windowHandle, int key, int scancode, int action, int mods)
{
	static_cast<Window*>(glfwGetWindowUserPointer(windowHandle))->key_callback(key, scancode, action, mods);
}

void mouse_button_callback_func(GLFWwindow* windowHandle, int button, int action, int mods)
{
	static_cast<Window*>(glfwGetWindowUserPointer(windowHandle))->mouse_button_callback(button, action, mods);
}

void mouse_callback_func(GLFWwindow* windowHandle, double xpos, double ypos)
{
	static_cast<Window*>(glfwGetWindowUserPointer(windowHandle))->mouse_callback(xpos, ypos);
}

void window_pos_callback_func(GLFWwindow* windowHandle, int xpos, int ypos)
{
	static_cast<Window*>(glfwGetWindowUserPointer(windowHandle))->window_pos_callback(xpos, ypos);
}

void window_focus_callback_func(GLFWwindow* windowHandle, int focused)
{
	static_cast<Window*>(glfwGetWindowUserPointer(windowHandle))->window_focus_callback(focused);
}

void window_iconify_callback_func(GLFWwindow* windowHandle, int iconified)
{
	static_cast<Window*>(glfwGetWindowUserPointer(windowHandle))->window_iconify_callback(iconified);
}

void window_cursor_entered_callback_func(GLFWwindow* windowHandle, int entered)
{
	static_cast<Window*>(glfwGetWindowUserPointer(windowHandle))->window_cursor_entered_callback(entered);
}

void mouse_scroll_callback_func(GLFWwindow* windowHandle, double xOffset, double yOffset)
{
	static_cast<Window*>(glfwGetWindowUserPointer(windowHandle))->mouse_scroll_callback(xOffset, yOffset);
}

CursorList::CursorList()
	: std::map<std::string, GLFWcursor*>{} {}

CursorList::~CursorList()
{
	auto it = begin();

	while(it != end())
	{
		glfwDestroyCursor(it->second);
		it = erase(it);
	}
}

void CursorList::add(const std::string& ref, GLFWcursor* cursor)
{
	emplace(ref, cursor);
}

void CursorList::remove(const std::string& ref)
{
	auto it = find(ref);

	if(it != end())
	{
		glfwDestroyCursor(it->second);
		it = erase(it);
	}
}

GLFWcursor* CursorList::get(const std::string& ref)
{
	auto it = find(ref);

	if(it == end())
	{
		return nullptr;
	}

	return it->second;
}

unsigned CursorList::getCount() const
{
	return size();
}

WindowSettings getDefaultWindowSettings()
{
	WindowSettings settings;

	settings.resizable = GLFW_TRUE;
	settings.visible = GLFW_TRUE;
	settings.decorated = GLFW_TRUE;
	settings.focused = GLFW_TRUE;
	settings.auto_iconify = GLFW_TRUE;
	settings.floating = GLFW_FALSE;
	settings.maximized = GLFW_FALSE;

	settings.cursorMode = CursorMode::NORMAL;

	settings.minXLimit = GLFW_DONT_CARE;
	settings.maxXLimit = GLFW_DONT_CARE;
	settings.minYLimit = GLFW_DONT_CARE;
	settings.maxYLimit = GLFW_DONT_CARE;

	settings.samples = 4;
	settings.vSync = GLFW_TRUE;

	settings.version_major = 4;
	settings.version_minor = 5;

	return settings;
}

Window::Window(const GLuint width, const GLuint height, const std::string& title, WindowSettings settings)
	: width{ width }, height{ height }, title{ title }
{
	// If this is the first window, we intitialize GLFW and GLEW aswell
	if (INSTANCE_COUNT++ == 0)
	{
		if (!glfwInit())
		{
			throw WindowRuntimeException("Failed to initialize GLFW");
		}

		glfwWindowHint(GLFW_RESIZABLE, settings.resizable);
		glfwWindowHint(GLFW_VISIBLE, settings.visible);
		glfwWindowHint(GLFW_DECORATED, settings.decorated);
		glfwWindowHint(GLFW_FOCUSED, settings.focused);
		glfwWindowHint(GLFW_AUTO_ICONIFY, settings.auto_iconify);
		glfwWindowHint(GLFW_FLOATING, settings.floating);
		glfwWindowHint(GLFW_MAXIMIZED, settings.maximized);

		glfwWindowHint(GLFW_SAMPLES, settings.samples); // 4x antialiasing
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, settings.version_major); // We want OpenGL 3.3
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, settings.version_minor);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

		windowHandle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

		if (windowHandle == nullptr) {
			glfwTerminate();
			throw WindowRuntimeException("Failed to open GLFW window");
		}

		glfwMakeContextCurrent(windowHandle); // Initialize GLEW
		glewExperimental = true; // Needed in core profile
		if (glewInit() != GLEW_OK) {
			glfwTerminate();
			throw WindowRuntimeException("Failed to initialize GLEW!");
		}

	}
	else // We just need to create a new window
	{
		glfwWindowHint(GLFW_RESIZABLE, settings.resizable);
		glfwWindowHint(GLFW_VISIBLE, settings.visible);
		glfwWindowHint(GLFW_DECORATED, settings.decorated);
		glfwWindowHint(GLFW_FOCUSED, settings.focused);
		glfwWindowHint(GLFW_AUTO_ICONIFY, settings.auto_iconify);
		glfwWindowHint(GLFW_FLOATING, settings.floating);
		glfwWindowHint(GLFW_MAXIMIZED, settings.maximized);

		glfwWindowHint(GLFW_SAMPLES, settings.samples); // 4x antialiasing
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, settings.version_major); // We want OpenGL 3.3
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, settings.version_minor);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

		windowHandle = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

		if (windowHandle == nullptr) {
			glfwTerminate();
			throw WindowRuntimeException("Failed to open GLFW window");
		}

		glfwMakeContextCurrent(windowHandle); // Set the current context to the new one.
	}

	setCursorMode(settings.cursorMode);

	glViewport(0, 0, width, height);

	// Get the current position
	int window_pos_x, window_pos_y;
	glfwGetWindowPos(windowHandle, &window_pos_x, &window_pos_y);
	xpos = static_cast<GLuint>(window_pos_x);
	ypos = static_cast<GLuint>(window_pos_y);

	glfwSetWindowSizeLimits(
		windowHandle,
		settings.minXLimit,
		settings.minYLimit,
		settings.maxXLimit,
		settings.maxYLimit);

	if (settings.vSync)
	{
		enableVSync();
	}
	else
	{
		disableVSync();
	}

	// Specify that this object will handle window
	glfwSetWindowUserPointer(windowHandle, this);

	// Setup callback functions
	glfwSetFramebufferSizeCallback(windowHandle, framebuffer_size_callback_func);
	glfwSetKeyCallback(windowHandle, key_callback_func);
	glfwSetMouseButtonCallback(windowHandle, mouse_button_callback_func);
	glfwSetCursorPosCallback(windowHandle, mouse_callback_func);
	glfwSetWindowPosCallback(windowHandle, window_pos_callback_func);
	glfwSetWindowFocusCallback(windowHandle, window_focus_callback_func);
	glfwSetWindowIconifyCallback(windowHandle, window_focus_callback_func);
	glfwSetCursorEnterCallback(windowHandle, window_cursor_entered_callback_func);
	glfwSetScrollCallback(windowHandle, mouse_scroll_callback_func);
}

Window::~Window()
{
	--INSTANCE_COUNT;
	glfwDestroyWindow(windowHandle);
	if (INSTANCE_COUNT == 0) // If this was the last window, terminate GLFW
	{
		glfwTerminate();
	}
}

glm::uvec2 Window::getDimensions() const noexcept
{
	int width, height;
	glfwGetWindowSize(windowHandle, &width, &height);
	return glm::vec2{ width, height };
}

GLuint Window::getWidth() const noexcept
{
	return width;
}

GLuint Window::getHeight() const noexcept
{
	return height;
}

GLfloat Window::getAspectRatio() const noexcept
{
	return static_cast<GLfloat>(width) / static_cast<GLfloat>(height);
}

GLFWwindow* Window::getHandle() const noexcept
{
	return windowHandle;
}

void Window::setDimensions(const GLuint newWidth, const GLuint newHeight)
{
	width = newWidth;
	height = newHeight;
	setContextCurrent();
	glViewport(0, 0, newWidth, newHeight);
}

void Window::setDimensions(const glm::uvec2& newDimensions)
{
	width = newDimensions.x;
	height = newDimensions.y;
	setContextCurrent();
	glViewport(0, 0, newDimensions.x, newDimensions.y);
}

GLuint Window::getXPos() const noexcept
{
	return xpos;
}

GLuint Window::getYPos() const noexcept
{
	return ypos;
}

glm::uvec2 Window::getPosition() const noexcept
{
	return glm::uvec2(xpos, ypos);
}

void Window::setTitle(const std::string& newTitle)
{
	title = newTitle;
	glfwSetWindowTitle(windowHandle, newTitle.c_str());
}

std::string Window::getTitle() const noexcept
{
	return title;
}

bool Window::shouldClose() const
{
	return (glfwWindowShouldClose(windowHandle) != 0);
}

void Window::requestClose() const
{
	glfwSetWindowShouldClose(windowHandle, GLFW_TRUE);
}

void Window::framebuffer_size_callback(int width, int height)
{
	this->width = width;
	this->height = height;
	setContextCurrent();
	glViewport(0, 0, width, height);

	WindowEvent ev;
	ev.type = EventType::RESIZED;

	ev.size.width = width;
	ev.size.height = height;

	eventQueue.push(ev);
}

void Window::key_callback(int key, int scancode, int action, int mods)
{
	// TODO: Remove and let client handle exit commands
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		requestClose();

	WindowEvent ev;
	ev.type = EventType::KEY_EVENT;
	ev.key.key = key;
	ev.key.scancode = scancode;

	switch (action)
	{
	case GLFW_PRESS:
		ev.key.action = Action::PRESS;
		break;
	case GLFW_REPEAT:
		ev.key.action = Action::REPEAT;
		break;
	case GLFW_RELEASE:
		ev.key.action = Action::RELEASE;
		break;
	}

	// Null out the struct
	ev.key.mods = { 0 };

	if (mods & GLFW_MOD_SHIFT)
		ev.key.mods.shift = 1;

	if (mods & GLFW_MOD_CONTROL)
		ev.key.mods.ctrl = 1;

	if (mods & GLFW_MOD_ALT)
		ev.key.mods.alt = 1;

	if (mods & GLFW_MOD_SUPER)
		ev.key.mods.super = 1;

	eventQueue.push(ev);
}

void Window::mouse_button_callback(int button, int action, int mods)
{
	WindowEvent ev;
	ev.type = EventType::MOUSE_KEY_EVENT;
	ev.mouse.button = button;

	switch (action)
	{
	case GLFW_PRESS:
		ev.mouse.action = Action::PRESS;
		break;
	case GLFW_REPEAT:
		ev.mouse.action = Action::REPEAT;
		break;
	case GLFW_RELEASE:
		ev.mouse.action = Action::RELEASE;
		break;
	}

	// Null out the struct
	ev.mouse.mods = { 0 };

	if (mods & GLFW_MOD_SHIFT)
		ev.mouse.mods.shift = 1;

	if (mods & GLFW_MOD_CONTROL)
		ev.mouse.mods.ctrl = 1;

	if (mods & GLFW_MOD_ALT)
		ev.mouse.mods.alt = 1;

	if (mods & GLFW_MOD_SUPER)
		ev.mouse.mods.super = 1;

	eventQueue.push(ev);
}

void Window::mouse_callback(double xpos, double ypos)
{
	WindowEvent ev;
	ev.type = EventType::MOUSE_MOVED_EVENT;
	ev.mouse.posx = xpos;
	ev.mouse.posy = ypos;
	eventQueue.push(ev);
}

void Window::window_pos_callback(int xpos, int ypos)
{
	this->xpos = xpos;
	this->ypos = ypos;
}

void Window::window_focus_callback(int focused)
{
	if (focused)
	{
		this->focused = true;

		WindowEvent ev;
		ev.type = EventType::GAINED_FOCUS;
		eventQueue.push(ev);
	}
	else
	{
		this->focused = false;

		WindowEvent ev;
		ev.type = EventType::LOST_FOCUS;
		eventQueue.push(ev);
	}
}

void Window::window_iconify_callback(int iconified)
{
	if (iconified)
	{
		this->iconified = true;

		WindowEvent ev;
		ev.type = EventType::ICONIFIED;
		eventQueue.push(ev);
	}
	else
	{
		this->iconified = false;

		WindowEvent ev;
		ev.type = EventType::RESTORED;
		eventQueue.push(ev);
	}
}

void Window::window_cursor_entered_callback(int entered)
{
	if (entered)
	{
		WindowEvent ev;
		ev.type = EventType::CURSOR_ENTER;
		eventQueue.push(ev);
	}
	else
	{
		WindowEvent ev;
		ev.type = EventType::CURSOR_EXIT;
		eventQueue.push(ev);
	}
}

void Window::mouse_scroll_callback(double xOffset, double yOffset)
{
	WindowEvent ev;
	ev.type = EventType::MOUSE_SCROLL_EVENT;
	ev.mouse.xOffset = xOffset;
	ev.mouse.yOffset = yOffset;
	eventQueue.push(ev);
}

void Window::display() const
{
	glfwSwapBuffers(windowHandle);
	glfwPollEvents();
}

void Window::setContextCurrent() const
{
	glfwMakeContextCurrent(windowHandle);
}

bool Window::pollEvent(WindowEvent& ev)
{
	if (eventQueue.empty())
		return false;

	ev = eventQueue.front();
	eventQueue.pop();
	return true;
}

void Window::enableVSync()
{
	glfwSwapInterval(1);
	VSyncActive = true;
}

void Window::disableVSync()
{
	glfwSwapInterval(0);
	VSyncActive = false;
}

bool Window::getVSyncStatus() const
{
	return VSyncActive;
}

void Window::iconify()
{
	glfwIconifyWindow(windowHandle);
	iconified = true;
}

void Window::restore()
{
	glfwRestoreWindow(windowHandle);
	iconified = false;
}

bool Window::hasFocus() const
{
	return focused;
}

void Window::setCursorMode(CursorMode mode)
{
	switch (mode)
	{
	case CursorMode::NORMAL:
		glfwSetInputMode(windowHandle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		break;
	case CursorMode::HIDDEN:
		glfwSetInputMode(windowHandle, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		break;
	case CursorMode::DISABLED:
		glfwSetInputMode(windowHandle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		break;
	default:
		throw WindowLogicException("Invalid Cursor Mode");
	}

	cursorMode = mode;
}

CursorMode Window::getCursorMode() const
{
	return cursorMode;
}

void Window::registerCursor(const std::string& ref, GLFWcursor* cursor)
{
	cursorList.add(ref, cursor);
}

void Window::registerCursor(const std::string& ref, GLFWimage* image, int xHot, int yHot)
{
	GLFWcursor* cursor = glfwCreateCursor(image, xHot, yHot);

	registerCursor(ref, cursor);
}

void Window::registerCursor(const std::string& ref, unsigned char* pixels, int width, int height, int xHot, int yHot)
{
	GLFWimage image;
	image.width = width;
	image.height = height;
	image.pixels = pixels;

	registerCursor(ref, &image, xHot, yHot);
}

void Window::unregisterCursor(const std::string& ref)
{
	cursorList.remove(ref);
}

GLuint Window::getCursorCount() const
{
	return cursorList.getCount();
}

void Window::setCursor(const std::string& ref)
{
	
	GLFWcursor* cursor = cursorList.get(ref);

	if (cursor == nullptr)
		throw WindowRuntimeException("Requested cursor not registered");

	glfwSetCursor(windowHandle, cursor);

	currentCursor = ref;
}

std::string Window::getCursor() const
{
	return currentCursor;
}
