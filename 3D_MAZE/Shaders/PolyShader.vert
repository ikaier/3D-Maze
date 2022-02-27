#version 330

layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;
layout (location=3) in mat4 instanceMatrix;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;

void main()
{
	gl_Position=projection*view*instanceMatrix*vec4(aPos,1.0);
	Normal = mat3(transpose(inverse(instanceMatrix)))*aNormal;
}