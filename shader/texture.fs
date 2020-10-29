#version 450 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D lightTexture;
uniform sampler2D worldGeometry;
uniform sampler2D shadowTexture;

void main()
{
    vec4 lightColor = texture(lightTexture, TexCoords);
    vec4 worldColor = texture(worldGeometry, TexCoords);
    vec4 outColor;

    outColor = lightColor * worldColor;

    FragColor = outColor;
}