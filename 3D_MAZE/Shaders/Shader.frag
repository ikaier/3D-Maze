#version 330

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;
in vec4 FragPosLightSpace;

const int MAX_POINT_LIGHTS=100;
uniform vec2 viewPort;

struct LightCommon{
	vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
	float constant;
	float linear;
	float exponent;
};

struct pointLight{
	LightCommon base;
	vec3 position;
};

struct SpotLight
{
	pointLight base;
	vec3 spotdirection;
	float edge;
	
};

struct Material{
	float specularIntensity;
	float shininess;
};

uniform int pointLightCount;
uniform sampler2D ourTexture;
uniform sampler2D lightTexture;
uniform sampler2D flashShadowMap;

uniform pointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;

uniform vec3 viewPos;

float FlashShadowCalculation(vec4 fragPosLightSpace)
{
	//perspective divide
	vec3 projCoords=fragPosLightSpace.xyz/fragPosLightSpace.w;
	
	//transform to [1,0] range
	projCoords=projCoords*0.5+0.5;
	
	//get closest depth value from light's perspective
	float closestDepth=texture(flashShadowMap,projCoords.xy).r;
	
	//get depth of current fragment from light's perspective
	float currentDepth=projCoords.z;
	//compare
	float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;
	
	return shadow;
}

vec4 CalcLightByDirection(LightCommon light,vec3 direction){
	vec4 ambientColor = vec4 (light.color, 1.0f ) * light.ambientIntensity;
	float diffuseFactor = max( dot(normalize(Normal), normalize(direction)), 0.0f);
    //diffuse
	vec4 diffuseColor = vec4(light.color* light.diffuseIntensity*diffuseFactor, 1.0f) ;
    //specular
	vec4 specularColor=vec4(0,0,0,0);
	if(diffuseFactor>0.0f){
		vec3 fragToEye=normalize(viewPos-FragPos);
		//vec3 reflectedVertex=normalize(reflect(-direction,normalize(Normal)));
        vec3 halfwayDir=normalize(direction+fragToEye);
		float specularFactor=dot(normalize(Normal),halfwayDir);
		if(specularFactor>0.0f){
			specularFactor=pow(specularFactor, material.shininess);
			specularColor=vec4(light.color*material.specularIntensity*specularFactor,1.0f);
		}
	}
	
	//return (ambientColor + (1.0-shadow)*(diffuseColor+specularColor)); 
	return (ambientColor + (1.0)*(diffuseColor+specularColor)); //+specularColor
}

vec4 CalcPointLight(pointLight pLight)
{
    //attenuation
		vec3 direction= pLight.position-FragPos;
		float distance = length(direction);
    
    
		direction = normalize(direction);
		vec4 color = CalcLightByDirection(pLight.base,direction);
		float attenuation=pLight.base.exponent*distance*distance+pLight.base.linear*distance+pLight.base.constant;
		return (color/attenuation);
}

vec4 CalcSpotLight(SpotLight sLight){
	vec3 rayDirection = normalize(FragPos-sLight.base.position);
	float slFactor=dot(rayDirection,sLight.spotdirection);

	if(slFactor>sLight.edge){
		vec4 color=CalcPointLight(sLight.base);
		return color*(1.0f-(1.0f-slFactor)*(1.0f/(1.0f-sLight.edge)));
	}else{
		return vec4(0,0,0,0);
}
}



vec4 CalcPointLights(){
	vec4 totalColor=vec4(0,0,0,1);
	for(int i=0;i<pointLightCount;i++)
	{
		totalColor+=CalcPointLight(pointLights[i]);
	}
	return totalColor;
}

vec4 CalcSpotLights(){
	vec4 totalColor=vec4(0,0,0,0);
	vec2 fragCoord=gl_FragCoord.xy/viewPort*vec2(1.0,-1.0);
	float shadow=FlashShadowCalculation(FragPosLightSpace);
	totalColor+=(1.0-shadow)*CalcSpotLight(spotLight);//*texture(lightTexture,fragCoord);
	
	return totalColor;
}



void main()
{
	
	vec4 finalColor=CalcPointLights();
	finalColor += CalcSpotLights();
	
	//finalColor*=
	FragColor=texture(ourTexture,TexCoord)*finalColor;

	//gamma correction
	float gamma = 2.2;
    FragColor.rgb = pow(FragColor.rgb, vec3(1.0/gamma));
	
	//shadow calculate
	
	
	
}
