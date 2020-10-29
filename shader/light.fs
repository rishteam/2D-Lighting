#version 450 core
out vec4 FragColor;

uniform vec4 lightColor;
uniform vec2 lightPos;
uniform float lightRadius;
uniform float constant;
uniform float linear;
uniform float quadratic;

void main()
{
    float distance = length(lightPos - gl_FragCoord.xy);
//    if(distance > lightRadius)
//        discard;
    //  1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
//    float attenuation =  2.0 / (1.0 + 10.0 * (distance / lightRadius));

    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));

    vec4 color = vec4(attenuation, attenuation, attenuation, pow(attenuation, 3)) * lightColor;

    FragColor = color;
}
