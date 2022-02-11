#version 330

out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform float ambientIntensity;
uniform vec3 lightColor;

void main()
{
	vec4 finalColor=vec4(ambientIntensity*lightColor,1.0);
	FragColor=texture(ourTexture,TexCoord)*finalColor;
}