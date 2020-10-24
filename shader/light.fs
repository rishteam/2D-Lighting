#version 450 core
out vec4 FragColor;

uniform vec4 lightColor;
uniform vec2 lightPos;
uniform float lightRadius;

void main()
{
    float distance = length(lightPos - gl_FragCoord.xy);
    float attenuation =  1.0 / (1.0 + 10.0 * (distance / lightRadius));
//    float attenuation =  1 / distance;
    vec4 color = vec4(attenuation, attenuation, attenuation, pow(attenuation, 3)) * lightColor;

    FragColor = color;
}
