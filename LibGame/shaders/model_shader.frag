#version 460 core

in vec2 TexCoord0;

out vec4 FragColor;

uniform sampler2D gSampler;

void main()
{
    FragColor = texture(gSampler, TexCoord0);
}
