#version 450 core
out vec4 FragColor;

uniform vec4 ourColor;
uniform sampler2D tex;

void main()
{
    FragColor = ourColor;
}