#version 330 core
out vec4 outColor;

uniform float colorVariable;

void main()
{
   outColor = vec4(colorVariable,colorVariable, colorVariable, 1.0);
}