#version 330

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;


uniform sampler2D ourTexture;
uniform float ambientIntensity;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main()
{
	vec3 norm=normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff=max(dot(norm,lightDir),0.0);
	vec3 diffuse=diff*lightColor;
	vec4 finalColor=vec4(ambientIntensity*lightColor+diffuse,1.0);
	FragColor=texture(ourTexture,TexCoord)*finalColor;
}