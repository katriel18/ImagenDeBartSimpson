#version 330 core
out vec4 outColor;

uniform float colorVariable;

void main()
{
   outColor = vec4(0.0, 1.0-colorVariable, 0.0, 1.0);
}