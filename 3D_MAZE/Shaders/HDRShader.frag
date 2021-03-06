#version 330

out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform float exposure;
uniform bool menuState;

const float offset = 1.0 / 200.0;  

void main()
{             

    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

float kernel[9] = float[](
    1.0 / 16, 2.0 / 16, 1.0 / 16,
    2.0 / 16, 4.0 / 16, 2.0 / 16,
    1.0 / 16, 2.0 / 16, 1.0 / 16  
);
    
    vec3 sampleTex[9];
	const float gamma = 2.2;
	
	
	    for(int i = 0; i < 9; i++)
		{
			sampleTex[i] = vec3(texture(scene, TexCoords.st + offsets[i]));
		}
		vec3 hdrColor = vec3(0.0);
		for(int i = 0; i < 9; i++)
			hdrColor += sampleTex[i] * kernel[i];



		vec3 hdrColor0 = texture(scene, TexCoords).rgb;
	
	if(!menuState){
		hdrColor=hdrColor0;
	}
	vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;
    hdrColor += bloomColor; // additive blending
    // tone mapping
    vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
	result = pow(result, vec3(1.0 / gamma));
    FragColor =vec4(result, 1.0);
}