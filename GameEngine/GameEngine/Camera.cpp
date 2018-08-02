#include "Camera.h"

using glm::vec3;

Camera::Camera(GLuint windowWidth, GLuint windowHeight)
{
    frustum = new Frustum();
    setCameraPosition(vec3(0, 0, 2.2), false);
    setSceneCenter(vec3(0, 0, 0), false);
    setCameraUp(vec3(0, 1, 0), true);
}

Camera::~Camera()
{
    delete frustum;
}

vec3
Camera::getCameraPosition() const
{
    return frustum->getCameraPosition();
}

vec3
Camera::getCameraUp() const
{
    return frustum->getCameraUp();
}

vec3
Camera::getSceneCenter() const
{
    return frustum->getCameraLookAt();
}

Frustum* 
Camera::getFrustum()
{
    return frustum;
}

void 
Camera::setCameraPosition(vec3 &position, bool updateImmediately)
{
    frustum->setCameraPosition(position, updateImmediately);
}

void 
Camera::setCameraUp(vec3 &up, bool updateImmediately)
{
    frustum->setCameraUp(up, updateImmediately);
}

void 
Camera::setSceneCenter(vec3 &center, bool updateImmediately)
{
    frustum->setCameraLookAt(center, updateImmediately);
}

vec3 
Camera::getRightNormal()
{
    frustum->updateFrustum();
    return     glm::normalize(glm::cross(frustum->getCameraDirection(),
        frustum->getCameraUp()));
}

void 
Camera::moveLeft(float distance)
{
    vec3 direction = getRightNormal();
    direction *= -distance;
    setCameraPosition(getCameraPosition() + direction);
    setSceneCenter(getSceneCenter() + direction);
}

void 
Camera::moveRight(float distance)
{
    vec3 direction = getRightNormal();
    direction *= distance;
    setCameraPosition(getCameraPosition() + direction);
    setSceneCenter(getSceneCenter() + direction);
}

void 
Camera::moveUp(float distance)
{
    vec3 direction = glm::normalize(getCameraUp());
    direction *= distance;
    setCameraPosition(getCameraPosition() + direction);
    setSceneCenter(getSceneCenter() + direction);
}

void 
Camera::moveDown(float distance)
{
    vec3 direction = glm::normalize(getCameraUp());
    direction *= -distance;
    setCameraPosition(getCameraPosition() + direction);
    setSceneCenter(getSceneCenter() + direction);

}

void 
Camera::moveForward(float distance)
{
    vec3 direction = glm::normalize(frustum->getCameraDirection());
    direction *= distance;
    setCameraPosition(getCameraPosition() + direction);
    setSceneCenter(getSceneCenter() + direction);
}

void 
Camera::moveBackward(float distance)
{
    vec3 direction = glm::normalize(frustum->getCameraDirection());
    direction *= -distance;
    setCameraPosition(getCameraPosition() + direction);
    setSceneCenter(getSceneCenter() + direction);
}