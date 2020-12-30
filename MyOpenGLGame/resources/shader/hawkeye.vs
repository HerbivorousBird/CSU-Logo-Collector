#version 330 core
layout (location = 0) in vec2 aPos;

out vec4 color;

uniform mat4 model;
uniform vec4 acolor;

void main()
{
	gl_Position = model * vec4(aPos,0,2.2);
	color = acolor;
}