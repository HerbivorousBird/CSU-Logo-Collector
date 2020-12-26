#version 330 core
out vec4 FragColor;

in vec3 TexCoord;

// texture samplers
uniform samplerCube texture1;
uniform sampler2D texture2;

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	FragColor = texture(texture1, TexCoord);
	
	//FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
}