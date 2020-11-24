#version 450 core
out vec4 FragColor;

in vec2 lightPos;

uniform vec4 lightColor;
uniform float lightRadius;
uniform float constant;
uniform float linear;
uniform float quadratic;
uniform float disMul;
uniform float zoomx;
uniform float zoomy;

void main()
{
    vec3 ndc = vec3(((gl_FragCoord.x/1280) * 2 -1), (gl_FragCoord.y/720) * 2 -1, gl_FragCoord.z * 2 -1) ;
    ndc = vec3(ndc.x * zoomx, ndc.y * zoomy, ndc.z);
    vec2 light = vec2(lightPos.x * zoomx, lightPos.y * zoomy);
//
    float distance = length(light - ndc.xy) * 50;
//
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));
//
    vec4 color = vec4(attenuation, attenuation, attenuation, pow(attenuation, 3)) * lightColor;
//    vec4 color = vec4(lightPos.x, 0, 0, 1);

    FragColor = color;
}
