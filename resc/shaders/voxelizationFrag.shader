#version 450 core

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
	float emissivity;
	float diffuseReflectivity;
	float specularReflectivity;
};

struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float constant;
	float linear;
	float quadratic;
};

in vec3 fragNormal;
in vec3 fragPos;
in vec2 fragTexCoords;

uniform vec3 view_pos;

uniform Material material;
uniform Light light;

uniform sampler2D texUnit;
layout(RGBA8) uniform image3D voxGrid;

float calculateAttenuation(float dist, Light light)
{
	return 1.0f / (light.constant + light.linear * dist + light.quadratic * pow(dist, 2));
}

void main()
{
	// Ambient color calculation
	vec3 ambient = light.ambient * material.ambient;

	// Diffuse color calculation
	vec3 normNorm = normalize( fragNormal );
	vec3 light_dir = normalize( light.position - fragPos );
	float diff = max( dot( normNorm, light_dir), 0.0 );
	vec3 diffuse = light.diffuse * ( diff * material.diffuse );

	// Specular light calculation
	vec3 view_dir = normalize( view_pos - fragPos );
	vec3 reflect_dir = reflect( -light_dir, normNorm );
	float spec = pow( max( dot( view_dir, reflect_dir ), 0.0f), material.shininess );
	vec3 specular = light.specular * (spec * material.specular);

	// Texture coordinates and attenuation
	vec4 objColor = texture(texUnit, fragTexCoords);
	float attenuation = calculateAttenuation(length(light.position - fragPos), light);

	// Add them
	vec4 result = objColor * vec4( attenuation * ( ambient + diffuse + specular ) + material.emissivity * material.diffuse, 1.f);

	result.r = min(result.r, 1.0f);
	result.g = min(result.g, 1.0f);
	result.b = min(result.b, 1.0f);

	// Upload result to (correct) voxel in voxel grid
	ivec3 dim = imageSize(voxGrid);
	vec3 voxelPos = 0.5f * fragPos + vec3(0.5f); // Map from -1->1 to 0->1 in 3D
	imageStore(voxGrid, ivec3(dim * voxelPos), result);
}