#include "FirstPersonCamera.h"

using glm::vec2;
using glm::vec3;

FirstPersonCamera::FirstPersonCamera(vec3 cameraSpeed, vec2 mouseSensitivity, 
    GLuint windowWidth, GLuint windowHeight) :
    Camera(windowWidth, windowHeight)
{
    m_cameraSpeed = cameraSpeed;
    m_mouseSensitivity = mouseSensitivity;
}

void 
FirstPersonCamera::moveLeft(float deltaTime)
{
    Camera::moveLeft(deltaTime * m_cameraSpeed.x);
}

void 
FirstPersonCamera::moveRight(float deltaTime)
{
    Camera::moveRight(deltaTime * m_cameraSpeed.x);
}

void 
FirstPersonCamera::moveUp(float deltaTime)
{
    Camera::moveUp(deltaTime * m_cameraSpeed.y);
}

void 
FirstPersonCamera::moveDown(float deltaTime)
{
    Camera::moveDown(deltaTime * m_cameraSpeed.y);
}

void 
FirstPersonCamera::moveForward(float deltaTime)
{
    Camera::moveForward(deltaTime * m_cameraSpeed.z);
}

void 
FirstPersonCamera::moveBackward(float deltaTime)
{
    Camera::moveBackward(deltaTime * m_cameraSpeed.z);
}

void
FirstPersonCamera::moveCursor(vec2 newMousePosition)
{
    if (m_initialCursorPositionRetrieved)
    {
        float xOffset = newMousePosition.x - m_lastMousePosition.x;
        float yOffset = m_lastMousePosition.y - newMousePosition.y;
        xOffset *= m_mouseSensitivity.x;
        yOffset *= m_mouseSensitivity.y;

        m_yaw += xOffset;
        m_pitch += yOffset;

        if (m_pitch > 89.0f)
            m_pitch = 89.0f;
        if (m_pitch < -89.0f)
            m_pitch = -89.0f;

        updateCameraVectors();
    }
    else
    {
        m_initialCursorPositionRetrieved = true;
    }
    m_lastMousePosition = newMousePosition;
}

void 
FirstPersonCamera::updateCameraVectors()
{
    vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    frustum->setCamera(getCameraPosition(), getCameraPosition() +
        glm::normalize(front), vec3(0, 1, 0));
}
