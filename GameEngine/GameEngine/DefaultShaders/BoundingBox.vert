#version 330 core
  
layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;

out vec4 inColor;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    gl_Position = projectionMatrix * modelViewMatrix * vec4(position.x, position.y, position.z, 1.0);
    inColor = color;
}
