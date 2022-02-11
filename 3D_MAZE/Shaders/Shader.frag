#version 330

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

struct Light{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	float constant;
	float linear;
	float quadratic;
};
uniform Light light;
uniform sampler2D ourTexture;
uniform float shininess;

uniform vec3 viewPos;

void main()
{
	vec3 ambient=light.ambient;
	vec3 norm=normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff=max(dot(norm,lightDir),0.0);
	vec3 diffuse=diff*light.diffuse;
	
	vec3 viewDir=normalize(viewPos-FragPos);
	vec3 reflectDir=reflect(-lightDir,norm);
	float spec=pow(max(dot(viewDir,reflectDir),0.0),shininess);
	vec3 specular=light.specular*spec;
	
	float distance = length(light.position-FragPos);
	float attenuation=1.0/(light.constant+light.linear*distance+light.quadratic*(distance*distance));
	ambient*=attenuation;
	diffuse*=attenuation;
	specular*=attenuation;
	
	vec4 finalColor=vec4(ambient+diffuse+specular,1.0);
	FragColor=texture(ourTexture,TexCoord)*finalColor;
}