#pragma once
#include "Frustum.h"
#include <GL/glew.h>

class Camera
{
public:
	/**
	* This creates a camera looking at the origin and positioned 15 units along
	* the positive Z axis.  UP is along the Y axis.
	*/
	Camera(GLuint windowWidth, GLuint windowHeight);
	~Camera();

	glm::vec3 getCameraPosition() const;
	glm::vec3 getCameraUp() const;
	glm::vec3 getSceneCenter() const;
	Frustum* getFrustum();
	glm::vec3 getRightNormal();

	void setCameraPosition(glm::vec3 &position, bool updateImmediately = true);
	void setCameraUp(glm::vec3 &up, bool updateImmediately = true);
	void setSceneCenter(glm::vec3 &center, bool updateImmediately = true);

	void moveLeft(float dist);
	void moveRight(float dist);
	void moveUp(float dist);
	void moveDown(float dist);
	void moveForward(float dist);
	void moveBackward(float dist);

protected:
	Frustum *frustum;
};