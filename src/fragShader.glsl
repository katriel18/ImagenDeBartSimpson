#version 330 core
out vec4 outColor;

uniform float colorVariable;
uniform float colorVariable2;
void main()
{
   
    outColor = vec4(colorVariable,colorVariable, colorVariable-colorVariable2, 1.0);
   //outColor = vec4(colorVariable,colorVariable, colorVariable, 1.0);
}