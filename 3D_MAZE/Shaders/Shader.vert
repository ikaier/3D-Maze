#version 330
layout (location=0) in vec3 aPos;
layout (location=1) in vec2 aTexCoord;
layout (location=2) in vec3 aNormal;
layout (location=3) in mat4 instanceMatrix;


out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;
out vec4 FragPosLightSpace;

//uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpaceMatrix;

void main()
{
	gl_Position = projection * view * instanceMatrix * vec4(aPos,1.0f);
	TexCoord = aTexCoord;
	Normal = mat3(transpose(inverse(instanceMatrix)))*aNormal;
	FragPos = vec3(instanceMatrix*vec4(aPos,1.0));
	FragPosLightSpace=lightSpaceMatrix*vec4(vec3(instanceMatrix * vec4(aPos,1.0f)),1.0);
}