#version 330

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;


uniform sampler2D ourTexture;
uniform float ambientIntensity;
uniform float diffuseIntensity;
uniform float specularIntensity;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	vec3 norm=normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff=max(dot(norm,lightDir),0.0);
	vec3 diffuse=diff*lightColor;
	
	vec3 viewDir=normalize(viewPos-FragPos);
	vec3 reflectDir=reflect(-lightDir,norm);
	float spec=pow(max(dot(viewDir,reflectDir),0.0),4);
	vec3 specular=specularIntensity*spec*lightColor;
	vec4 finalColor=vec4(ambientIntensity*lightColor+diffuse*diffuseIntensity+specular,1.0);
	FragColor=texture(ourTexture,TexCoord)*finalColor;
}