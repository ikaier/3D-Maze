#version 330

layout (location=0) out vec4 FragColor;
layout (location=1) out vec4 BrightColor;

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

struct OmniShadowMap
{
	samplerCube shadowMap;
	float farPlane;
};

struct Material{
	float specularIntensity;
	float shininess;
};

uniform int pointLightCount;
uniform int ShadowPointLightCount;
uniform sampler2D ourTexture;
uniform sampler2D lightTexture;
uniform sampler2D flashShadowMap;
uniform OmniShadowMap omniShadowMaps[MAX_POINT_LIGHTS];

uniform pointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;

uniform vec3 viewPos;

vec3 sampleOffsetDirections[20]=vec3[]
(
	vec3( 1, 1, 1), vec3( 1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1),
	vec3( 1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
	vec3( 1, 1, 0), vec3( 1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
	vec3( 1, 0, 1), vec3( -1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1),
	vec3( 0, 1, 1), vec3( 0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1)
);

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
	
	//bais
	float bais =max(0.001*(1.0-dot(Normal,spotLight.spotdirection)),0.0001);
	
	float shadow=0.0;
	vec2 texelSize=1.0 / textureSize(flashShadowMap , 0);
	for (int x = -1; x <=1; ++x)
	{
		for(int y =-1; y<=1;++y){
			float pcfDepth = texture(flashShadowMap,projCoords.xy + vec2(x,y)*texelSize).r;
			shadow += currentDepth - bais > pcfDepth ? 1.0:0.0;
		}
	}
	shadow/= 9.0;
	
	if(projCoords.z>1.0f)
		shadow=0.0;

	return shadow;
}



vec4 CalcLightByDirection(LightCommon light,vec3 direction,float shadowFactor){
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
	
	return (ambientColor + (1.0-shadowFactor)*(diffuseColor+specularColor)); 
	//return (ambientColor + (1.0)*(diffuseColor+specularColor)); //+specularColor
}

vec4 CalcPointLight(pointLight pLight,float shadowFactor)
{
    //attenuation
		vec3 direction= pLight.position-FragPos;
		float distance = length(direction);
    
    
		direction = normalize(direction);
		vec4 color = CalcLightByDirection(pLight.base,direction,shadowFactor);
		float attenuation=pLight.base.exponent*distance*distance+pLight.base.linear*distance+pLight.base.constant;
		return (color/attenuation);
}

vec4 CalcSpotLight(SpotLight sLight,float shadowFactor){
	vec3 rayDirection = normalize(FragPos-sLight.base.position);
	float slFactor=dot(rayDirection,sLight.spotdirection);

	if(slFactor>sLight.edge){
		vec4 color=CalcPointLight(sLight.base,shadowFactor);
		return color*(1.0f-(1.0f-slFactor)*(1.0f/(1.0f-sLight.edge)));
	}else{
		return vec4(0,0,0,0);
	}
}

float WallLightShadowCal(pointLight plight,int sIndex)
{
	if(sIndex>ShadowPointLightCount){
		return 0.0;
	}
	vec3 fragToLight=FragPos-plight.position;
	
	
	float currentDepth=length(fragToLight);
	float shadow=0.0;
	float bias=0.005;
	int samples=20;
	
	float viewDistance=length(viewPos-FragPos);
	float diskRadius=(1.0+(viewDistance/omniShadowMaps[sIndex].farPlane))/600.0;
	
	for(int i=0;i<samples;i++){
		float closestDepth = texture(omniShadowMaps[sIndex].shadowMap,fragToLight+sampleOffsetDirections[i]*diskRadius).r;
				closestDepth *= omniShadowMaps[sIndex].farPlane;
				if(currentDepth - bias > closestDepth){
					shadow+=1.0;
				}
	}

	
	shadow /=float(samples);
	return shadow;
	
}

vec4 CalcPointLights(){
	vec4 totalColor=vec4(0,0,0,1);
	for(int i=0;i<pointLightCount;i++)
	{
		float shadowFactor=WallLightShadowCal(pointLights[i],i);
		totalColor+=CalcPointLight(pointLights[i],shadowFactor);
	}
	
	return totalColor;
}

vec4 CalcSpotLights(){
	vec4 totalColor=vec4(0,0,0,0);
	vec2 fragCoord=gl_FragCoord.xy/viewPort*vec2(1.0,-1.0);
	float shadowFactor=FlashShadowCalculation(FragPosLightSpace);
	totalColor+=CalcSpotLight(spotLight,shadowFactor)*texture(lightTexture,fragCoord);
	
	return totalColor;
}



void main()
{
	
	vec4 finalColor=CalcPointLights();
	finalColor += CalcSpotLights();
	
	//if(TexCoord==vec2(0.0,0.0)){
	//	FragColor=vec4(1.0,1.0,0.0,1.0)*finalColor;
	//}else{
	//	FragColor=texture(ourTexture,TexCoord)*finalColor;
	//}
	FragColor=texture(ourTexture,TexCoord)*finalColor;
	//gamma correction
	//float gamma = 2.2;
    //FragColor.rgb = pow(FragColor.rgb, vec3(1.0/gamma));
	float brightness=dot(vec3(FragColor),vec3(0.2126,0.7152,0.0722));
	if(brightness>2.0){
		BrightColor=vec4(vec3(FragColor),1.0);
	}else{
		BrightColor=vec4(0.0,0.0,0.0,1.0);
	}
	FragColor=vec4(vec3(FragColor),1.0);
	
	
	
	
}
