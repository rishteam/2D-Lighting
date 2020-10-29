#version 450 core
out vec4 FragColor;

uniform vec4 ourColor;

void main()
{
    vec4 ambientColor = vec4(1, 0, 0, 1);
    FragColor = ourColor;
}