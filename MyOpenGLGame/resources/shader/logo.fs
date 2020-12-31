#version 330 core
out vec4 FragColor;

in vec2 texCoord;
in vec3 position;
in vec3 normal;
in float alpha;

uniform sampler2D texture1;

uniform vec3 viewPos;



void main()
{
	vec3 lightAmbient = vec3(0.5f, 0.5f, 0.5f);
	vec3 lightDiffuse = vec3(0.5f, 0.5f, 0.5f);
	vec3 lightSpecular = vec3(1.0f, 1.0f, 1.0f);

	// material attributes
	vec3 matAmbient = vec3(0.099412, 0.103529, 0.27451);
	vec3 matDiffuse= vec3(0.110392, 0.118627, 0.603725);
	vec3 matSpecular = vec3(0.22157, 0.241176, 0.997843);
	float matShininess = 30.897400;

	vec4 texColor = texture(texture1, texCoord);
    if(texColor.a < 0.1)
        discard;


    // ambient
    vec3 ambient = lightAmbient*matAmbient;

   // diffuse
    vec3 lightDir = normalize(vec3(0,-1,1));
    vec3 normDir = normalize(normal);
    float dotLN = abs(dot(lightDir, normDir)); //通过指定绝对值使得正反面相同
    vec3 diffuse = lightDiffuse *  matDiffuse * dotLN;

    // specular
    vec3 reflectDir = normalize(reflect(-lightDir, normDir));
    vec3 viewDir = normalize(viewPos - position);
    float dotRV = abs(dot(reflectDir, viewDir));
    vec3 specular = lightSpecular * matSpecular* pow(dotRV, matShininess);

    // phong model result
    vec3 color = ambient + diffuse + specular;
	FragColor = vec4(color, alpha);
}