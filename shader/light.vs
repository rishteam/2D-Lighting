#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 pos;

out vec2 lightPos;
uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(aPos, 1.0);
    lightPos = vec4(pos, 0, 1).xy;
}