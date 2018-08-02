#pragma once
#include "Frustum.h"
#include <GL/glew.h>

class Camera
{
public:
	Camera(GLuint windowWidth, GLuint windowHeight);
	virtual ~Camera();

	glm::vec3 getCameraPosition() const;
	glm::vec3 getCameraUp() const;
	glm::vec3 getSceneCenter() const;
	Frustum* getFrustum();
	glm::vec3 getRightNormal();

	void setCameraPosition(glm::vec3 &position, bool updateImmediately = true);
	void setCameraUp(glm::vec3 &up, bool updateImmediately = true);
	void setSceneCenter(glm::vec3 &center, bool updateImmediately = true);

	virtual void moveLeft(float dist);
	virtual void moveRight(float dist);
	virtual void moveUp(float dist);
	virtual void moveDown(float dist);
	virtual void moveForward(float dist);
	virtual void moveBackward(float dist);

protected:
	Frustum *frustum;
};