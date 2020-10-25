#version 450 core
out vec4 FragColor;

uniform vec4 ourColor;

void main()
{
    FragColor = ourColor * vec4(0.0, 0.0, 0.0, 1);
}