#version 450 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D worldGeometry;

void main()
{
//    vec4 color = mix(texture(screenTexture, TexCoords), texture(worldGeometry, TexCoords), 0.5);
//    FragColor = color;
    FragColor =texture(screenTexture, TexCoords) * texture(worldGeometry, TexCoords);
//    FragColor = texture(screenTexture, TexCoords);
}