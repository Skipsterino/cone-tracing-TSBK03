#version 330 core

//=============================================================================
// Structs
//=============================================================================

struct Light
{
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

//=============================================================================
// Input
//=============================================================================

in vec3 normal;
in vec3 FragPos;
in vec2 texCoords;

//=============================================================================
// Output
//=============================================================================

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 brightColor;

//=============================================================================
// Uniforms
//=============================================================================

// Texture to sample main texture from.
uniform sampler2D 	textureUnit;

// Position of the camera during the render pass.
uniform vec3 		viewPos;

// Number of lights in scene
uniform int numLights;

// Light data. TODO: Variable length buffer with SSBO
uniform Light lights[32];

// Shadow depth cube maps
uniform samplerCube depthMaps[32];

// Material Properties
uniform Material material;

// Far plane used by shadows
uniform float far_plane;

//=============================================================================
// Functions
//=============================================================================

float calculateShadow(int i)
{
	vec3 fragToLight = FragPos - lights[i].position;
		
	float currentDepth = length(fragToLight);
	
	float cosTheta = clamp(dot(normalize(normal), normalize(-fragToLight)), 0, 1);
	
	float bias = 0.005*tan(acos(cosTheta));
	bias = clamp(bias, 0.0, 0.01);
	
	float shadow = 0.0;
	float samples = 2.0;
	float offset = 0.1;
	
	for(float x = -offset; x < offset; x+= offset/(samples*0.5))
	{
		for(float y = -offset; y < offset; y+= offset/(samples*0.5))
		{
			for(float z = -offset; z < offset; z+= offset/(samples*0.5))
			{
				float closestDepth = texture(depthMaps[i], fragToLight + vec3(x,y,z)).r;
				closestDepth *= far_plane;
				if(currentDepth - bias > closestDepth)
				{
					shadow += 1.0;
				}
			}
		}
	}
	
	shadow /= pow(samples, 3.0);
	
	return shadow;
}

float calculateAttenuation(float distance, Light light)
{
	return 1.0f/(light.constant + light.linear * distance + light.quadratic * pow(distance, 2.0f));
}

//=============================================================================
// Main
//=============================================================================

void main()
{
	// Calculate useful variables
	vec3 norm = normalize(normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	
	// Variable to store light result.
	vec3 resColor = vec3(0,0,0);
	for(int i = 0; i < numLights; ++i)
	{
		vec3 lightDirection = normalize(lights[i].position - FragPos);

		// Ambient light calculation
		vec3 ambientColor = lights[i].ambient*material.ambient;
	
		// Diffuse light calculation
		float diff = max(dot(norm, lightDirection), 0.0);
		vec3 diffuseColor = lights[i].diffuse * diff * material.diffuse;
	
		// Specular light calculation
		vec3 reflectDir = reflect(-lightDirection, norm);
		float lightDistance = length(lights[i].position - FragPos);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
		vec3 specularColor = lights[i].specular * spec * material.specular;
	
		// Calculate Shadow factor.
		float shadow = calculateShadow(i);
		
		shadow = clamp(shadow, 0.0, 1.0);
		
		// Calulate Attenuatuon factor.
		float attenuation = calculateAttenuation(lightDistance, lights[i]);
	
		// Add the results to the resulting light color
		resColor += (ambientColor + (1.0 - shadow)*(diffuseColor + specularColor))*attenuation;
	}
	
	// Sample texture to get object color.
	vec3 objColor = texture(textureUnit, texCoords).rgb;
	
	vec3 outputColor = objColor * resColor;
	
	float brightness = dot(outputColor, vec3(0.2126,0.7152,0.0722));
	
	// Output color.
	fragColor = vec4(outputColor, 1.0);
	
	if(brightness > 1.0)
		brightColor = vec4(outputColor, 1.0);
	else
		brightColor = vec4(0.0, 0.0, 0.0, 1.0);
}

//=============================================================================
// End of file
//=============================================================================