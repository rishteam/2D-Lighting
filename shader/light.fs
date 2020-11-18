#version 450 core
out vec4 FragColor;

in vec2 lightPos;

uniform vec4 lightColor;
uniform float lightRadius;
uniform float constant;
uniform float linear;
uniform float quadratic;
uniform float disMul;

void main()
{
    vec3 ndc = vec3(((gl_FragCoord.x/1280) * 2 -1), (gl_FragCoord.y/720) * 2 -1, gl_FragCoord.z * 2 -1) ;
//
    float distance = length(lightPos.xy - ndc.xy) * 150;
//
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));
//
    vec4 color = vec4(attenuation, attenuation, attenuation, pow(attenuation, 3)) * lightColor;
//    vec4 color = vec4(distance.x, 0, 0, 1);

    FragColor = color;
}
