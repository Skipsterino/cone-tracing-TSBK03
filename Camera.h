/**
 * @file	Camera.h
 * @Author	Joakim Bertils
 * @date	2017-02-09
 * @brief	Camera class
 */

#pragma once

#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define SMOOTH_MOVEMENT_CONSTANT 0.3f

/**
 * @brief Enumeration of the directions the camera can travel.
 */
enum CameraMovement
{
	/**
	 * @brief Forward direction
	 */
	FORWARD,

	/**
	 * @brief Backward direction.
	 */
	BACKWARD,

	/**
	 * @brief Left direction.
	 */
	LEFT,

	/**
	 * @brief Right direction.
	 */
	RIGHT,

	/**
	* @brief Absolute Up direction.
	*/
	ABSUP,

	/**
	* @brief Absolute Down direction
	*/
	ABSDOWN
};

/**
 * @brief Default camera position.
 */
const glm::vec3 POSITION = glm::vec3(0.f, 0.f, 0.f);

/**
 * @brief Default camera up direction
 */
const glm::vec3 UP = glm::vec3(0.f, 1.f, 0.f);

/**
 * @brief Default camera yaw value.
 */
const GLfloat YAW = 0.f;

/**
 * @brief Default camera pitch value.
 */
const GLfloat PITCH = 0.0f;

/**
 * @brief Default camera speed.
 */
const GLfloat SPEED = 1.f;

/**
 * @brief Default camera mouse sensitivity.
 */
const GLfloat SENSITIVITY = 0.15f;

/**
 * @brief Class representing a camera.
 */
class Camera
{
public:

	/**
	 * @brief Camera Constructor
	 * @param position The starting position of the camera.
	 * @param up The starting UP vector.
	 * @param yaw The starting yaw value.
	 * @param pitch The starting pitch value.
	 */
	explicit Camera(
		glm::vec3 position = POSITION,
		glm::vec3 up = UP,
		GLfloat yaw = YAW,
		GLfloat pitch = PITCH
		);

	/**
	 * @brief Gets the view matrix.
	 * @return View Matrix.
	 */
	glm::mat4 getViewMatrix() const;

	/**
	 * @brief Processes the keyboard.
	 * @param direction The direction to move along.
	 * @param timeDelta The time to simulate.
	 */
	void processKeyboard(CameraMovement direction, GLfloat timeDelta);
	
	/**
	 * @brief Process mouse movement
	 * @param xOffset The mouse x offset relative to last value.
	 * @param yOffset The mouse y offset relative to last value.
	 * @param constrainPitch If the pitch should be constrained
	 */
	void processMouseMovement(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch = true);

	/**
	 * @brief Position getter.
	 * @return The current camera position.
	 */
	glm::vec3 getPosition() const;

	/**
	 * @brief Position Setter
	 * @param newPos New camera position.
	 */
	void setPosition(const glm::vec3& newPos);
	
	/**
	 * @brief Front vector getter. 
	 * @return The current Front vector.
	 */
	glm::vec3 getFrontVector() const;

	/**
	 * @brief Up vector getter.
	 * @return The current Up vector.
	 */
	glm::vec3 getUpVector() const;

	/**
	 * @brief Right vector getter.
	 * @return The current Right vector.
	 */
	glm::vec3 getRightVector() const;

	/**
	 * @brief World up vector getter.
	 * @return The current World Up vector.
	 */
	glm::vec3 getWorldUpVector() const;

	/**
	 * @brief Yaw Getter
	 * @return Current yaw value.
	 */
	GLfloat getYaw() const;
	
	/**
	* @brief Pitch Getter
	* @return Current pitch value.
	*/
	GLfloat getPitch() const;

	/**
	 * @brief Movement speed getter.
	 * @return Current movement speed.
	 */
	GLfloat getMovementSpeed() const;

	/**
	 * @brief Mouse sensitivity getter.
	 * @return Current mouse sensitivity.
	 */
	GLfloat getMouseSensitivity() const;

	void update();

	void tempPrintCoords();
private:

	/**
	 * @brief Camera position. 
	 */
	glm::vec3 position;

	/**
	 * @brief Camera front vector.
	 */
	glm::vec3 front;

	/**
	 * @brief Camera up vector.
	 */
	glm::vec3 up;
	
	/**
	 * @brief Camera right vector.
	 */
	glm::vec3 right;

	/**
	 * @brief World Up Vector.
	 */
	glm::vec3 worldUp;

	/**
	* @brief Yaw value
	*/
	GLfloat yaw;

	/**
	 * @brief Pitch value
	 */
	GLfloat pitch;

	/**
	 * @brief Current movement speed.
	 */
	GLfloat movementSpeed;

	/**
	 * @brief Current mouse sensitivity.
	 */
	GLfloat mouseSensitivity;

	/**
	* @brief Accumulated mouse xoffset.
	*/
	GLfloat accxoffset;

	/**
	* @brief Accumulated mouse yoffset.
	*/
	GLfloat accyoffset;

	/**
	* @brief Accumulated position left/right offset.
	*/
	GLfloat acclroffset;

	/**
	* @brief Accumulated position up/down offset.
	*/
	GLfloat accudoffset;

	/**
	* @brief Accumulated position front/back offset.
	*/
	GLfloat accfboffset;

	/**
	 * @brief Updates the camera vectors after an update.
	 */
	void updateCameraVectors();
};