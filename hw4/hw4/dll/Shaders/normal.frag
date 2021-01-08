#version 430

uniform sampler2D texture;

in vec4 color;
out vec4 fragcolor;

void main()
{
	fragcolor = color;
} 