#version 330

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

const int MAX_POINT_LIGHTS=3;

struct PointLightCommon{
	vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
	float constant;
	float linear;
	float exponent;
};

struct pointLight{
	vec3 position;
};

struct Material{
	float specularIntensity;
	float shininess;
};

uniform int pointLightCount;
uniform sampler2D ourTexture;

uniform PointLightCommon pointLightProperty;
uniform pointLight pointLights[MAX_POINT_LIGHTS];
uniform Material material;

uniform vec3 viewPos;

vec4 CalcLightByDirection(PointLightCommon light,vec3 direction){
	vec4 ambientColor = vec4 (light.color, 1.0f ) * light.ambientIntensity;
	float diffuseFactor = max( dot(normalize(Normal), normalize(direction)), 0.0f);
	vec4 diffuseColor = vec4(light.color* light.diffuseIntensity*diffuseFactor, 1.0f) ;

	vec4 specularColor=vec4(0,0,0,0);
	if(diffuseFactor>0.0f){
		vec3 fragToEye=normalize(viewPos-FragPos);
		vec3 reflectedVertex=normalize(reflect(direction,normalize(Normal)));
		
		float specularFactor=dot(fragToEye,reflectedVertex);
		if(specularFactor>0.0f){
			specularFactor=pow(specularFactor, material.shininess);
			specularColor=vec4(light.color*material.specularIntensity*specularFactor,1.0f);
		}
	}
	return (ambientColor +diffuseColor + specularColor);
}

vec4 CalcPointLight(pointLight pLight)
{
		vec3 direction=FragPos - pLight.position;
		float distance = length(direction);
		direction = normalize(direction);
		
		vec4 color = CalcLightByDirection(pointLightProperty,direction);
		float attenuation=pointLightProperty.exponent*distance*distance+pointLightProperty.linear*distance+pointLightProperty.constant;
		return (color/attenuation);
}

vec4 CalcPointLights()
{
	vec4 totalColor=vec4(0,0,0,0);
	for(int i=0;i<pointLightCount;i++)
	{
		totalColor+=CalcPointLight(pointLights[i]);
	}
	return totalColor;
}

void main()
{
	
	vec4 finalColor=CalcPointLights();
	FragColor=texture(ourTexture,TexCoord)*finalColor;
}