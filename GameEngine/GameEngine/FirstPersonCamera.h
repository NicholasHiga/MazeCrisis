#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>

#include "Camera.h"

class FirstPersonCamera final : public Camera
{
public:
    FirstPersonCamera(glm::vec3 cameraSpeed, glm::vec2 mouseSensitivty, 
        GLuint windowWidth, GLuint windowHeight);
    
    void moveCursor(glm::vec2 newMousePosition);
    void moveLeft(float deltaTime);
    void moveRight(float deltaTime);
    void moveUp(float deltaTime);
    void moveDown(float deltaTime);
    void moveForward(float deltaTime);
    void moveBackward(float deltaTime);
   
    float m_yaw{ 90.0f };
    float m_pitch{ 0.0f };
    glm::vec2 m_lastMousePosition{ 0, 0 };
    glm::vec3 m_cameraSpeed{ 1.0f, 1.0f, 1.0f };
    glm::vec2 m_mouseSensitivity{ 0, 0 };
    bool m_initialCursorPositionRetrieved{ false };

private:
    void updateCameraVectors();
};