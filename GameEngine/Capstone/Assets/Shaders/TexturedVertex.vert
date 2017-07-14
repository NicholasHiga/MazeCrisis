#version 330 core
  
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;

out vec2 texCoords;

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    gl_Position = projectionMatrix * modelViewMatrix * vec4(position.x, position.y, position.z, 1.0);
	texCoords = vec2(uv.x, 1.0 - uv.y);
}