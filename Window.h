/**
 * @file	Window.h
 * @Author	Joakim Bertils
 * @date	2017-02-08
 * @brief	Window related structs and classes.
 */

 // TODO: Text input events
 // TODO: Maybe Joystick
 // TODO: Maybe clipboard stuff
 // TODO: Maybe Path Drop stuff

#pragma once

#include <stdexcept>
#include <queue>
#include <map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

/**
  * @brief Exception thrown when runtime errors occur.
  */
class WindowRuntimeException : std::runtime_error
{
public:
	using std::runtime_error::runtime_error;
};

/**
* @brief Exception thrown when logic errors occur.
*/
class WindowLogicException : std::logic_error
{
public:
	using std::logic_error::logic_error;
};

/**
 * @brief Enum containing all event type codes.
 */
enum class EventType
{
	/**
	 * @brief Type code for keypress/release event
	 */
	KEY_EVENT = 0,

	/**
	 * @brief Type code for mouse movement event
	 */
	MOUSE_MOVED_EVENT,

	/**
	 * @brief Type code for mouse keypress/keyrelease event.
	 */
	MOUSE_KEY_EVENT,

	/**
	 * @brief Type code for mouse scroll event.
	 */
	MOUSE_SCROLL_EVENT,

	/**
	 * @brief Type code for focus gain event
	 */
	GAINED_FOCUS,

	/**
	 * @brief Type code for focus lost event
	 */
	LOST_FOCUS,

	/**
	 * @brief Type code for window iconified event.
	 */
	ICONIFIED,

	/**
	 * @brief Type code for window restored event.
	 */
	RESTORED,

	/**
	 * @brief Type code for cursor enter event.
	 */
	CURSOR_ENTER,

	/**
	 * @brief Type code for cursor exit event.
	 */
	CURSOR_EXIT,

	/**
	 * @brief Type code for window resize event.
	 */
	RESIZED,

	/**
	 * @brief Total number of used event codes
	 */
	NUM_EVENTS
};

/**
 * @brief Enumerations of all the actions registered for buttons.
 */
enum class Action
{
	/**
	 * @brief Key pressed action.
	 */
	PRESS = 0,

	/**
	 * @brief Key repeated action.
	 */
	REPEAT,

	/**
	 * @brief Key released action.
	 */
	RELEASE,

	/**
	 * @brief Number of actions available.
	 */
	NUM_ACTIONS
};

/**
 * @brief Packing of the modifier keys
 */
struct Modifiers
{
	/**
	 * @brief Shift key status.
	 */
	uint32_t shift : 1;

	/**
	 * @brief Control key status.
	 */
	uint32_t ctrl : 1;

	/**
	 * @brief Alt key status.
	 */
	uint32_t alt : 1;

	/**
	 * @brief Super key status.
	 */
	uint32_t super : 1;

	/**
	 * @brief Reserved. Should be 0.
	 */
	uint32_t reserved : 28;
};

/**
 * @brief Struct containing data representing an event.
 */
struct WindowEvent
{
	/**
	 * @brief Event type
	 */
	EventType type;

	union
	{
		/**
		 * @brief Data belonging to keyboard events.
		 */
		struct
		{
			/**
			 * @brief Virtual key code
			 *
			 * Only valid in KEY_EVENT.
			 */
			int key;

			/**
			* @brief Scan code of the key.
			*
			* Only valid in KEY_EVENT.
			*/
			int scancode;

			/**
			* @brief Action performed (press/release)
			*
			* Only valid in KEY_EVENT.
			*/
			Action action;

			/**
			* @brief Modifier keys
			*
			* Only valid in KEY_EVENT.
			*/
			Modifiers mods;
		} key;

		/**
		 * @brief Data belonging to mouse events.
		 */
		struct
		{
			union
			{
				struct
				{
					/**
					 * @brief Mouse position along the x-axis.
					 *
					 * Only valid in MOUSE_MOVED_EVENT.
					 */
					double posx;

					/**
					* @brief Mouse position along the y-axis.
					*
					* Only valid in MOUSE_MOVED_EVENT.
					*/
					double posy;
				};

				struct
				{
					/**
					* @brief Virtual mouse key code.
					*
					* Only valid in MOUSE_KEY_EVENT.
					*/
					int button;

					/**
					* @brief Action performed (press/release)
					*
					* Only valid in MOUSE_KEY_EVENT.
					*/
					Action action;

					/**
					* @brief Modifier keys
					*
					* Only valid in MOUSE_KEY_EVENT.
					*/
					Modifiers mods;
				};

				struct
				{
					/**
					* @brief Offset along the x-axis.
					*
					* Only valid in MOUSE_SCROLL_EVENT.
					*/
					double xOffset;

					/**
					* @brief Offset along the y-axis.
					*
					* Only valid in MOUSE_SCROLL_EVENT.
					*/
					double yOffset;
				};
			};
		} mouse;

		/**
		 * @brief Data belonging to size events.
		 */
		struct
		{
			/**
			 * @brief New width of the framebuffer
			 *
			 * Only valid in RESIZED event.
			 */
			uint32_t width;

			/**
			 * @brief New height of the framebuffer
			 *
			 * Only valid in RESIZED event.
			 */
			uint32_t height;
		} size;
	};
};

/**
 * @brief Enumeration of the available cursor modes.
 */
enum class CursorMode
{
	/**
	 * @brief Normal cursor.
	 */
	NORMAL = 0,

	/**
	 * @brief Hidden cursor.
	 */
	HIDDEN,

	/**
	 * @brief Cursor disabled.
	 */
	DISABLED,

	/**
	 * @brief Number of cursor modes available.
	 */
	NUM_MODES
};

/**
 * @brief List of cursors mapped to a string.
 */
class CursorList : std::map<std::string, GLFWcursor*>
{
public:
	/**
	 * @brief Creates an empty list of cursors.
	 */
	CursorList();

	/**
	 * @brief Clears the list and deletes all cursors with glfwDestroyCursor().
	 */
	~CursorList();

	/**
	 * @brief Adds a new cursor to the list.
	 * @param ref Reference string to which the cursor should be mapped.
	 * @param cursor Cursor object containging cursor information.
	 */
	void add(const std::string& ref, GLFWcursor* cursor);

	/**
	 * @brief Removes a cursor from the list.
	 * @param ref Reference to remove.
	 */
	void remove(const std::string& ref);

	/**
	 * @brief Finds the cursor with reference @ref.
	 * @param ref The reference to find.
	 * @return Ptr to GLFWcursor object if found, otherwise nullptr.
	 */
	GLFWcursor* get(const std::string& ref);

	/**
	 * @brief Gets the number of registered cursors.
	 * @return Cursor count.
	 */
	unsigned getCount() const;
};

/**
 * @brief Struct containing customizable window settings.
 */
struct WindowSettings
{
	//============================================================================
	// Window Related Settings
	//============================================================================

	/**
	 * @brief Window is resizable
	 */
	GLboolean resizable;

	/**
	 * @brief Window is visible at startup
	 */
	GLboolean visible;

	/**
	 * @brief Window will include borders and window bar
	 */
	GLboolean decorated;

	/**
	 * @brief Window will have focus at startup
	 */
	GLboolean focused;

	/**
	 * @brief Window will automatically be iconified if focus is lost in fullscreen
	 */
	GLboolean auto_iconify;

	/**
	 * @brief Window will always be on top
	 */
	GLboolean floating;

	/**
	 * @brief Window will initially be maximized
	 */
	GLboolean maximized;

	/**
	 * @brief Minimum width of window.
	 */
	GLuint minXLimit;

	/**
	 * @brief Maximum width of window.
	 */
	GLuint maxXLimit;
	/**
	 * @brief Minimum height of window.
	 */
	GLuint minYLimit;

	/**
	 * @brief Maximum height of window.
	 */
	GLuint maxYLimit;

	/**
	 * @brief Cursor mode.
	 */
	CursorMode cursorMode;

	//============================================================================
	// Framebuffer Related Settings
	//============================================================================

	/**
	 * @brief How many samples should multisampling use.
	 */
	GLuint samples;

	/**
	 * @brief Sync redraw rate with screen redraw rate.
	 */
	GLboolean vSync;

	//============================================================================
	// Context Related Settings
	//============================================================================

	/**
	 * @brief Major number of desired version
	 */
	GLuint version_major;

	/**
	 * @brief Minor number of desired verison
	 */
	GLuint version_minor;
};

/**
 * @brief Default Window Settings
 *
 * Constructs a struct containing the default values of the window settings.
 *
 * @return Struct containing default settings.
 */
WindowSettings getDefaultWindowSettings();

/**
 * @brief Representation of a window
 */
class Window
{
public:

	/**
	 * @brief Default Constructor
	 * @note  Deleted
	 */
	Window() = delete;

	/**
	* @brief Copy Constructor
	* @note  Deleted
	*/
	Window(const Window&) = delete;

	/**
	* @brief Move Constructor
	* @note  Deleted
	*/
	Window(Window&&) = delete;

	/**
	 * @brief Constructor for the window
	 *
	 * Creates a new window from the given parameters. Initializes GLFW and GLEW the first time a window is created.
	 *
	 * @param width Width of the window
	 * @param height Height of the window
	 * @param title Title of the window
	 * @param settings Desired settings.
	 */
	Window(
		const GLuint width,
		const GLuint height,
		const std::string& title,
		WindowSettings settings = getDefaultWindowSettings());

	/**
	 * @brief Destructor for the window
	 *
	 * Removes and cleans up after the window.
	 * If this is the last window, GLFW will be terminated.
	 */
	~Window();

	/**
	 * @brief Copy assignment operator
	 * @note  Deleted
	 */
	const Window& operator=(const Window&) = delete;

	/**
	* @brief Move assignment operator
	* @note  Deleted
	*/
	Window& operator=(Window&&) = delete;

	/**
	 * @brief Get window dimentions
	 * @return Width and Height packed into a vector
	 */
	glm::uvec2 getDimensions() const noexcept;

	/**
	 * @brief Get Window Width
	 * @return Width of the Window
	 */
	GLuint getWidth() const noexcept;

	/**
	 * @brief Get Window Height
	 * @return Height of the Window.
	 */
	GLuint getHeight() const noexcept;

	/**
	 * @brief Gets the ratio Width/Height.
	 * @return Aspect ratio.
	 */
	GLfloat getAspectRatio() const noexcept;

	/**
	 * @brief Get Window Handle
	 * @return The GLFW window handle
	 */
	GLFWwindow* getHandle() const noexcept;

	/**
	 * @brief Sets New Dimensions
	 * @param newWidth New Width of the Window
	 * @param newHeight New Height of the Window
	 */
	void setDimensions(const GLuint newWidth, const GLuint newHeight);

	/**
	 * @brief Sets new Dimensions
	 * @param newDimensions New Dimensions of the Window
	 */
	void setDimensions(const glm::uvec2& newDimensions);

	/**
	 * @brief Get X Position
	 * @return X-position
	 */
	GLuint getXPos() const noexcept;

	/**
	 * @brief Get Y Position
	 * @return Y-position
	 */
	GLuint getYPos() const noexcept;

	/**
	 * @brief Get Position of window.
	 * @return Position packed as a vector
	 */
	glm::uvec2 getPosition() const noexcept;

	/**
	 * @brief Sets new Title
	 * @param newTitle New Title of the Window
	 */
	void setTitle(const std::string& newTitle);

	/**
	 * @brief Gets the current title
	 * @return Current window title
	 */
	std::string getTitle() const noexcept;

	/**
	 * @brief Window Should Close flag
	 * @return True if window should close.
	 */
	bool shouldClose() const;

	/**
	 * @brief Sets the Window Should Close Flag
	 */
	void requestClose() const;

	/**
	 * @brief Displays the content of the framebuffer in the window.
	 *
	 * This is done by swapping buffers. Also polls events related to this window.
	 */
	void display() const;

	/**
	 * @brief Sets this window as the current context.
	 */
	void setContextCurrent() const;

	/**
	 * @brief Polls for any window events
	 * @param ev Reference to struct where data will be stored.
	 * @return False if there is no more events to handle.
	 */
	bool pollEvent(WindowEvent& ev);

	/**
	 * @brief Enables vertical sync for this window.
	 */
	void enableVSync();

	/**
	 * @brief Disables vertical sync for this window
	 */
	void disableVSync();

	/**
	 * @brief Gets the vertical sync status.
	 * @return True if vertical sync is enabled.
	 */
	bool getVSyncStatus() const;

	/**
	 * @brief Minimizes the window.
	 */
	void iconify();

	/**
	 * @brief Restores the window from a minimized state.
	 */
	void restore();

	/**
	 * @brief Gets focus status
	 * @return True if window is focused.
	 */
	bool hasFocus() const;

	/**
	 * @brief Sets the cursor mode
	 * @param mode New Cursor mode.
	 */
	void setCursorMode(CursorMode mode);

	/**
	 * @brief Gets the current cursor mode.
	 * @return Current cursor mode.
	 */
	CursorMode getCursorMode() const;

	/**
	 * @brief Registers a new cursor to the window.
	 * @param ref Reference to the cursor.
	 * @param cursor Cursor object ptr.
	 */
	void registerCursor(const std::string& ref, GLFWcursor* cursor);

	/**
	 * @brief Registers a new cursor to the window.
	 * @param ref Reference to the cursor.
	 * @param image Image object ptr.
	 * @param xHot Hotspot along the x-axis.
	 * @param yHot Hotspot along the y-axis.
	 */
	void registerCursor(const std::string& ref, GLFWimage* image, int xHot, int yHot);

	/**
	 * @brief Registers a new cursor to the window.
	 * @param ref Reference to the cursor.
	 * @param pixels Ptr to the pixel array. Format should be 32-bit RGBA data and a total size of 4*width*height bytes.
	 * @param width Width of the source image.
	 * @param height Height of the source image.
	 * @param xHot Hotspot along the x-axis.
	 * @param yHot Hotspot along the y-axis.
	 */
	void registerCursor(const std::string& ref, unsigned char* pixels, int width, int height, int xHot, int yHot);

	/**
	 * @brief Unregisters a cursor
	 * @param ref Reference to the cursor.
	 */
	void unregisterCursor(const std::string& ref);

	/**
	 * @brief Gets the number of registered cursors.
	 * @return Cursor count.
	 */
	GLuint getCursorCount() const;

	/**
	 * @brief Sets the current cursor. 
	 * 
	 * @note The cursor must be registered with @registerCursor() before.
	 * 
	 * @param ref Reference to the cursor.
	 */
	void setCursor(const std::string& ref);

	/**
	 * @brief Gets the currently active cursor.
	 * @return Reference to the cursor.
	 */
	std::string getCursor() const;

private:

	/**
	 * @brief Framebuffer size callback handler
	 * @param windowHandle Handle to the window being resized
	 * @param width New Width of window
	 * @param height New Height of window
	 */
	friend void framebuffer_size_callback_func(GLFWwindow* windowHandle, int width, int height);

	/**
	 * @brief Keyboard callback handler
	 * @param windowHandle Handle to the window getting the key press/release
	 * @param key Virtual keycode for the key
	 * @param scancode Scancode for the key
	 * @param action Action performed (press/release)
	 * @param mods Modifier keys pressed at the same time
	 */
	friend void key_callback_func(GLFWwindow* windowHandle, int key, int scancode, int action, int mods);

	/**
	 * @brief Mouse button callback handler
	 * @param windowHandle Handle to the window getting the mouse event.
	 * @param button The button which was pressed.
	 * @param action Action performed (press/release)
	 * @param mods Modifier keys pressed at the same time.
	 */
	friend void mouse_button_callback_func(GLFWwindow* windowHandle, int button, int action, int mods);

	/**
	 * @brief Mouse callback handler
	 * @param windowHandle Handle to the window getting the mouse update
	 * @param xpos New x-position of the mouse
	 * @param ypos New y-position of the mouse
	 */
	friend void mouse_callback_func(GLFWwindow* windowHandle, double xpos, double ypos);

	/**
	 * @brief Window position callback handler
	 * @param windowHandle Handle to the window being moved.
	 * @param xpos New x-position of the window.
	 * @param ypos New y-position of the window.
	 */
	friend void window_pos_callback_func(GLFWwindow* windowHandle, int xpos, int ypos);

	/**
	 * @brief Window focus change callback handler
	 * @param windowHandle Handle to the window being focused/unfocused.
	 * @param focused Non-zero if focused
	 */
	friend void window_focus_callback_func(GLFWwindow* windowHandle, int focused);

	/**
	 * @brief Window iconified status change callback handler.
	 * @param windowHandle Handle to the window being iconified/restored.
	 * @param iconified Non-zero if iconified
	 */
	friend void window_iconify_callback_func(GLFWwindow* windowHandle, int iconified);

	/**
	 * @brief Window cursor entered callback handler.
	 * @param windowHandle Handle to the window.
	 * @param entered Non-zero if cursor entered window.
	 */
	friend void window_cursor_entered_callback_func(GLFWwindow* windowHandle, int entered);

	/**
	 * @brief Mouse scroll callback handler
	 * @param windowHandle Handle to the window.
	 * @param xOffset Offset along the x-axis.
	 * @param yOffset Offset along the y-axis.
	 */
	friend void mouse_scroll_callback_func(GLFWwindow* windowHandle, double xOffset, double yOffset);

	/**
	 * @brief Class member callback for framebuffer change
	 * @param width New Width of window.
	 * @param height New Height of window.
	 */
	void framebuffer_size_callback(int width, int height);

	/**
	 * @brief Class member callback for key press/release
	 * @param key Virtual keycode for the key
	 * @param scancode Scancode for the key
	 * @param action Action performed (press/release)
	 * @param mods Modifier keys pressed at the same time
	 */
	void key_callback(int key, int scancode, int action, int mods);

	/**
	 * @brief Class member callback for mouse key press/release
	 * @param button Mouse key button
	 * @param action Action performed (press/release)
	 * @param mods Modifier keys pressed at the same time.
	 */
	void mouse_button_callback(int button, int action, int mods);

	/**
	 * @brief Class member callback for mouse movement
	 * @param xpos New x-position of the mouse
	 * @param ypos New y-position of the mouse
	 */
	void mouse_callback(double xpos, double ypos);

	/**
	 * @brief Class member callback for window position change
	 * @param xpos New x-position of the window.
	 * @param ypos New y-position of the window.
	 */
	void window_pos_callback(int xpos, int ypos);

	/**
	 * @brief Class member callback for window focus change.
	 * @param focused Non-zero if focused.
	 */
	void window_focus_callback(int focused);

	/**
	 * @brief Class member callback for window inconified status change
	 * @param iconified Non-zero if iconified.
	 */
	void window_iconify_callback(int iconified);

	/**
	 * @brief Class member callback for cursor enter/exit window event.s
	 * @param entered Non-zero if entered.
	 */
	void window_cursor_entered_callback(int entered);

	/**
	 * @brief Class member callback for mouse scroll event.
	 * @param xOffset Offset along the x-axis.
	 * @param yOffset Offset along the y-axis.
	 */
	void mouse_scroll_callback(double xOffset, double yOffset);

	/**
	 * @brief GLFW Window Handle
	 */
	GLFWwindow* windowHandle;

	/**
	 * @brief Window Event Queue
	 */
	std::queue<WindowEvent> eventQueue;

	/**
	 * @brief VSync status
	 */
	bool VSyncActive{ false };

	/**
	 * @brief Number of currently instantiated windows
	 */
	static int INSTANCE_COUNT;

	/**
	 * @brief Width of the Window.
	 */
	GLuint width;

	/**
	 * @brief Height of the window.
	 */
	GLuint height;

	/**
	 * @brief X-position of the window relative to the screen.
	 */
	GLuint xpos;

	/**
	 * @brief Y-position of the window relative to the screen.
	 */
	GLuint ypos;

	/**
	 * @brief The title of the window.
	 */
	std::string title;

	/**
	 * @brief True if window is focused.
	 */
	bool focused{ true };

	/**
	 * @brief True if the window is iconified.
	 */
	bool iconified{ false };

	/**
	 * @brief Current cursor mode.
	 */
	CursorMode cursorMode{ CursorMode::NORMAL };

	/**
	 * @brief Cursor list.
	 */
	CursorList cursorList{};

	/**
	 * @brief Currently active cursor.
	 */
	std::string currentCursor{};
};