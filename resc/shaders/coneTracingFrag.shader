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

in vec3 fragNorm;
in vec3 fragPos;
in vec2 texCoords;

out vec4 fragColor;

uniform int gridSize;
uniform int Mode;

uniform vec3 view_pos;

uniform Material material;
uniform Light light;

uniform sampler2D texUnit;
uniform sampler3D voxGrid;

float voxelSize = 1.f / gridSize;
float MAX_DISTANCE = distance(vec3(abs(fragPos)), vec3(-1.f));
vec3 fragNormNorm = normalize(fragNorm);

float calculateAttenuation(float dist)
{
	return 1.0f / (light.constant + light.linear * dist + light.quadratic * pow(dist, 2) + 0.3f * noise1(fragPos.x));
}

vec3 findOrthVec(vec3 u) {
	u = normalize(u);
	vec3 v = normalize(noise3(length(fragPos)));
	return abs(dot(u, v)) > 0.99999f ? cross(u, vec3(0, 1, 0)) : cross(u, v);
}

bool outOfBounds(const vec3 pos)
{ 
	return abs(pos.x) > 1 || abs(pos.y) > 1 || abs(pos.z) > 1;
}

vec3 castSpecularCone(vec3 from, vec3 dir) {
	dir = normalize(dir);

	const float offset = 2 * voxelSize;
	const float distStep = voxelSize;

	from += offset * fragNormNorm;
	float dist = offset;

	vec4 acc = vec4(0.0f);
	while (dist < MAX_DISTANCE && acc.a < 1.f) {
		vec3 curGridPos = from + dist * dir;
		if (outOfBounds(curGridPos)) break;

		float MMlevel = 0.1 * log2(1 + dist / voxelSize);
		vec4 voxel = textureLod(voxGrid, 0.5f * curGridPos + vec3(0.5f), min(MMlevel, 6.f));

		acc.rgb += 0.6 * voxel.rgb * (1 - acc.a);
		acc.a += 0.6 * voxel.a;
		dist += distStep * (1.0f + 0.15f * MMlevel);
	}
	return 1.0 * acc.rgb;
}

vec3 castDiffuseCone(vec3 from, vec3 dir)
{
	dir = normalize(dir);

	const float spread = 0.35;

	vec4 acc = vec4(0.0f);
	float dist = 0.05;
	vec3 curGridPos = from;
	while (dist < MAX_DISTANCE && acc.a < 1) {
		curGridPos = from + dist * dir;

		float l = (1 + spread * dist / voxelSize);
		float MMlevel = log2(l);

		vec4 voxel = textureLod(voxGrid, vec3(0.5f) + 0.5f * curGridPos, min(6, MMlevel));
		acc += 0.3 * voxel * pow(1 - voxel.a, 2);
		dist += MMlevel * voxelSize * 3;
	}
	return pow(acc.rgb * 2.0, vec3(1.5));
}

float castShadowCone()
{

	vec3 dir = light.position - fragPos; // direction from fragment to light position
	const float travelDist = length(dir) - 6.f * voxelSize;
	dir = normalize(dir);

	const float offset = 2 * voxelSize;
	const float distStep = voxelSize;

	vec3 from = fragPos + offset * fragNormNorm;
	float dist = offset;

	float shadowAcc = 0.f;
	vec3 curGridPos = from + dist * dir;
	while (dist < travelDist && shadowAcc < 1.f) {
		curGridPos = from + dist * dir;
		if (outOfBounds(curGridPos)) break;
		float MMlevel = 0.6 * log2(1 + dist / voxelSize);

		vec4 voxel1 = textureLod(voxGrid, 0.5f * curGridPos + vec3(0.5f), min(1.f + MMlevel, 6.f));
		vec4 voxel2 = textureLod(voxGrid, 0.5f * curGridPos + vec3(0.5f), 0.3f * min(1.f + MMlevel, 2.f));

		shadowAcc += 0.034f * voxel1.a + 0.09f * voxel2.a;

		dist += distStep * (1.0f + 0.15f * MMlevel) / 2;
	}

	return pow(0.7f * max(1.f - shadowAcc + 0.2f * noise1(fragPos.x), 0.f), 0.8);
}

vec3 indirectSpecularLight() 
{
	const vec3 viewDir = normalize(fragPos - view_pos);
	const vec3 reflection = normalize(reflect(viewDir, fragNormNorm));

	if (Mode == 4)
		return 0.8f * material.specular * castSpecularCone(fragPos, reflection);
	else 
		return 0.8f * material.specular * material.specularReflectivity * castSpecularCone(fragPos, reflection);
}

vec3 indirectDiffuseLight()
{
	// Find a orthonormal base from frag normal 
	const vec3 orth = normalize(findOrthVec(fragNormNorm));
	const vec3 orth2 = normalize(cross(orth, fragNormNorm));

	// Find start position of trace.
	const vec3 offset = fragNormNorm * 6.f * voxelSize; // to avoid 
	const vec3 from = fragPos + offset;

	vec3 acc = vec3(0);

	// front cone
	acc += castDiffuseCone(from, fragNormNorm);

	// 4 side cones
	acc += castDiffuseCone(from, orth);
	acc += castDiffuseCone(from, -orth);
	acc += castDiffuseCone(from, orth2);
	acc += castDiffuseCone(from, -orth2);

	// Return result.
	return 0.7f * pow(1.1f * acc * material.diffuse * material.diffuseReflectivity, vec3(0.9f));
}

vec3 directLight()
{
	// Ambient color calculation
	vec3 ambient = light.ambient*material.ambient;

	// Diffuse color calculation
	vec3 light_dir = normalize(light.position - fragPos);
	float diff = max(dot(fragNormNorm, light_dir), 0.0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	// Specular light calculation
	vec3 view_dir = normalize(view_pos - fragPos);
	vec3 reflect_dir = reflect(-light_dir, fragNormNorm);

	float spec = pow(max(dot(view_dir, reflect_dir), 0.0f), material.shininess);
	vec3 specular = light.specular * (spec * material.specular);

	float attenuation = calculateAttenuation(length(light.position - fragPos));
	
	// Add and return them
	return 0.9f * (attenuation * (ambient + diffuse + specular));
}


void main()
{
	vec4 objColor = texture(texUnit, texCoords);

	if (Mode == 0)
		fragColor.bgra = textureLod(voxGrid, 0.5f*(fragPos)+vec3(0.5f), 0.f);
	else if (Mode == 1)
		fragColor.bgra = objColor * vec4(directLight(), 1.f);
	else if (Mode == 2)
		fragColor.brga = vec4(vec3(1.f) * castShadowCone(), 1.f);
	else if (Mode == 3)
		fragColor.bgra = objColor * 1.5f * vec4(indirectDiffuseLight(), 1.f);
	else if (Mode == 4)
		fragColor.bgra = objColor * vec4(indirectSpecularLight(), 1.f) * 3.f;
	else if (Mode == 5)
		fragColor.bgra = objColor * vec4(0.7f * (indirectSpecularLight() + indirectDiffuseLight() + directLight()*castShadowCone()) + 0.8f * material.emissivity * material.diffuse, 1.f);
		
}