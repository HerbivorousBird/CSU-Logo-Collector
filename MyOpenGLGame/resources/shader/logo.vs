#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec3 position;
out vec3 normal;
out vec2 texCoord;
out float alpha;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float aalpha;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);;
	normal = mat3(transpose(inverse(model))) * vec3(0,0,1);
	texCoord = aTexCoord;
	alpha = aalpha;
}