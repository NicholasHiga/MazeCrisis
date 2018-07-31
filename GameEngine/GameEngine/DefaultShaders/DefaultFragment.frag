#version 330 core

in vec2 texCoords;

out vec4 color;

uniform sampler2D textureIn;

void main()
{
    color = texture(textureIn, texCoords);
    //color = vec4(0.5f, 0.5f, 0.5f, 1.0f);
} 
