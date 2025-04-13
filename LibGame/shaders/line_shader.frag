#version 460 core

out vec4 FragColor;

in vec4 v4Color;

void main()
{
    FragColor = vec4(v4Color);
}
