#version 450 core
out vec4 FragColor;

uniform vec4 lightColor;
uniform vec2 lightPos;
uniform float lightRadius;

void main()
{
    float distance = length(lightPos - gl_FragCoord.xy);
//    if(distance > lightRadius)
//        discard;
    float attenuation =  1.0 / (1.0 + 10.0 * (distance / lightRadius));
//    float attenuation =  1 / distance;
    vec4 color = vec4(attenuation, attenuation, attenuation, pow(attenuation, 1)) * lightColor;
//    vec4 color = vec4(lightColor.rgb, (1-distance) * lightColor.a);

    FragColor = color;
}
