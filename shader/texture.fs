#version 450 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D lightTexture;
uniform sampler2D worldGeometry;
uniform sampler2D shadowTexture;

void main()
{
//    vec4 color = mix(texture(lightTexture, TexCoords), texture(worldGeometry, TexCoords), 0.5);
//    FragColor = color;

    vec4 lightColor = texture(lightTexture, TexCoords);
    vec4 worldColor = texture(worldGeometry, TexCoords);
    vec4 shadowColor = texture(shadowTexture, TexCoords);

    // 有 shadow

    if(shadowColor.rgb == vec3(0, 0, 0) &&
        (worldColor.rgb != vec3(0, 0, 0) || lightColor.rgb != vec3(0, 0, 0)))
        FragColor = lightColor * worldColor;
    else
        FragColor = lightColor * worldColor * shadowColor;
//    FragColor = shadowColor;
}