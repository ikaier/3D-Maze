#version 330

out vec4 FragColor;
uniform vec4 Color;

void main()
{
	FragColor=Color;
	
	//gamma correction
	float gamma = 2.2;
    	FragColor.rgb = pow(FragColor.rgb, vec3(1.0/gamma));
}