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

    vec4 outColor = lightColor * worldColor;

//    outColor *= mix(outColor, 1.0f, 0.0);
//    outColor *= mix(0.5, 1.0, 0.0);

    FragColor = outColor;
}