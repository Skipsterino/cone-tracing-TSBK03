#version 450 core

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
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

#define TSQRT2 2.828427
#define SQRT2 1.414213
#define ISQRT2 0.707106

float VOXEL_SIZE = 1.f / gridSize;
float MAX_DISTANCE = distance(vec3(abs(fragPos)), vec3(-1.f));
vec3 fragNormNorm = normalize(fragNorm);

float calculateAttenuation(float dist)
{
	return 1.0f / (light.constant + light.linear * dist + light.quadratic * dist * dist + 0.3f * noise1(fragPos.x));
}

vec3 findOrthVec(vec3 u) {
	u = normalize(u);
	vec3 v = vec3(0.1690, 0.8451, 0.5070); // Random normalized vector
	return abs(dot(u, v)) > 0.99999f ? cross(u, vec3(0, 1, 0)) : cross(u, v);
}

bool isInsideCube(const vec3 p) { return abs(p.x) < 1 && abs(p.y) < 1 && abs(p.z) < 1; }

// Traces a specular voxel cone.
vec3 traceSpecularVoxelCone(vec3 from, vec3 dir) {
	dir = normalize(dir);

	const float OFFSET = 2 * VOXEL_SIZE;
	const float STEP = VOXEL_SIZE;

	from += OFFSET * fragNormNorm;
	float dist = OFFSET;

	vec4 acc = vec4(0.0f);
	while (dist < MAX_DISTANCE && acc.a < 1.f) {
		vec3 curGridPos = from + dist * dir;
		if (!isInsideCube(curGridPos)) break;

		float MMlevel = 0.1 * log2(1 + dist / VOXEL_SIZE);
		vec4 voxel = textureLod(voxGrid, 0.5f * curGridPos + vec3(0.5f), min(MMlevel, 6.f));
		float f = 1 - acc.a;
		//acc.rgb += 0.9 * voxel.rgb * voxel.a * f;
		//acc.a += 0.9 * voxel.a * f;
		acc.rgb += 0.6 * voxel.rgb * f;
		acc.a += 0.6 * voxel.a;
		dist += STEP * (1.0f + 0.125f * MMlevel);
	}
	return 1.0 * acc.rgb;
}

vec3 indirectSpecularLight() 
{
	const vec3 viewDir = normalize(fragPos - view_pos);
	const vec3 reflection = normalize(reflect(viewDir, fragNormNorm));
	return 0.5f * material.specular * traceSpecularVoxelCone(fragPos, reflection);
}

vec3 traceDiffuseVoxelCone(const vec3 from, vec3 dir) {
	dir = normalize(dir);

	const float CONE_SPREAD = 0.325;

	vec4 acc = vec4(0.0f);

	// Controls bleeding from close surfaces.
	// Low values look rather bad if using shadow cone tracing.
	// Might be a better choice to use shadow maps and lower this value.
	//float dist = 0.01953125;
	float dist = 0.05;
	// Trace.
	while (dist < MAX_DISTANCE && acc.a < 1) {
		vec3 curGridPos = from + dist * dir;
		curGridPos = 0.5f * (from + dist * dir) + vec3(0.5f);

		float l = (1 + CONE_SPREAD * dist / VOXEL_SIZE);
		float MMlevel = log2(l);
		float ll = (MMlevel + 1) * (MMlevel + 1);
		vec4 voxel = textureLod(voxGrid, curGridPos, min(5, MMlevel));
		acc += 0.075 * ll * voxel * pow(1 - voxel.a, 2);
		dist += ll * VOXEL_SIZE * 2;
	}
	return pow(acc.rgb * 2.0, vec3(1.5));
}

vec3 indirectDiffuseLight() {
	const float ANGLE_MIX = 0.7f; // Angle mix (1.0f => orthogonal direction, 0.0f => direction of normal).

	const float w[3] = { 1.0, 1.0, 1.0 }; // Cone weights.

	// Find a base for the side cones with the normal as one of its base vectors.
	const vec3 ortho = normalize(findOrthVec(fragNormNorm));
	const vec3 ortho2 = normalize(cross(ortho, fragNormNorm));

	// Find base vectors for the corner cones too.
	const vec3 corner = 0.5f * (ortho + ortho2);
	const vec3 corner2 = 0.5f * (ortho - ortho2);

	// Find start position of trace (start with a bit of offset).
	const vec3 N_OFFSET = fragNormNorm * (1 + 4 * ISQRT2) * VOXEL_SIZE;
	const vec3 C_ORIGIN = fragPos + N_OFFSET;

	// Accumulate indirect diffuse light.
	vec3 acc = vec3(0);

	// We offset forward in normal direction, and backward in cone direction.
	// Backward in cone direction improves GI, and forward direction removes
	// artifacts.
	const float CONE_OFFSET = -0.01;

	// Trace front cone
	acc += w[0] * traceDiffuseVoxelCone(C_ORIGIN + CONE_OFFSET * fragNormNorm, fragNormNorm);

	// Trace 4 side cones.
	const vec3 s1 = mix(fragNormNorm, ortho, ANGLE_MIX);
	const vec3 s2 = mix(fragNormNorm, -ortho, ANGLE_MIX);
	const vec3 s3 = mix(fragNormNorm, ortho2, ANGLE_MIX);
	const vec3 s4 = mix(fragNormNorm, -ortho2, ANGLE_MIX);

	acc += w[1] * traceDiffuseVoxelCone(C_ORIGIN + CONE_OFFSET * ortho, s1);
	acc += w[1] * traceDiffuseVoxelCone(C_ORIGIN - CONE_OFFSET * ortho, s2);
	acc += w[1] * traceDiffuseVoxelCone(C_ORIGIN + CONE_OFFSET * ortho2, s3);
	acc += w[1] * traceDiffuseVoxelCone(C_ORIGIN - CONE_OFFSET * ortho2, s4);

	// Trace 4 corner cones.
	const vec3 c1 = mix(fragNormNorm, corner, ANGLE_MIX);
	const vec3 c2 = mix(fragNormNorm, -corner, ANGLE_MIX);
	const vec3 c3 = mix(fragNormNorm, corner2, ANGLE_MIX);
	const vec3 c4 = mix(fragNormNorm, -corner2, ANGLE_MIX);

	//acc += w[2] * traceDiffuseVoxelCone(C_ORIGIN + CONE_OFFSET * corner, c1);
	//acc += w[2] * traceDiffuseVoxelCone(C_ORIGIN - CONE_OFFSET * corner, c2);
	//acc += w[2] * traceDiffuseVoxelCone(C_ORIGIN + CONE_OFFSET * corner2, c3);
	//acc += w[2] * traceDiffuseVoxelCone(C_ORIGIN - CONE_OFFSET * corner2, c4);

	// Return result.
	return 0.5f * acc * (material.diffuse + vec3(0.001f));
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
	return 0.9f * attenuation * (ambient + diffuse + specular);
}

float castShadowCone() 
{

	vec3 dir = light.position - fragPos; // direction from fragment to light position
	const float travelDist = length(dir) - 5.f * VOXEL_SIZE;
	dir = normalize(dir);

	const float OFFSET = 2 * VOXEL_SIZE;
	const float STEP = VOXEL_SIZE;

	vec3 from = fragPos + OFFSET * fragNormNorm;
	float dist = OFFSET;

	float shadowAcc = 0.f;
	vec3 curGridPos = from + dist * dir;
	while (dist < travelDist && shadowAcc < 1.f) {
		curGridPos = from + dist * dir;
		if (!isInsideCube(curGridPos)) break;
		float MMlevel = 0.3 * log2(1 + dist / VOXEL_SIZE);
		vec4 voxel = textureLod(voxGrid, 0.5f * curGridPos + vec3(0.5f), min(1.f + MMlevel, 6.f));
		

		shadowAcc += 0.1f * voxel.a;

		dist += STEP * (1.0f + 0.125f * MMlevel) / 2;
	}

	return max(1.f - shadowAcc + 0.2f * noise1(fragPos.x), 0.f);
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
		fragColor.bgra = objColor * vec4(indirectDiffuseLight(), 1.f);
	else if (Mode == 4)
		fragColor.bgra = objColor * vec4(indirectSpecularLight(), 1.f) * 3.f;
	else if (Mode == 5)
		fragColor.bgra = objColor * vec4(0.9f * ( indirectDiffuseLight() + directLight()*castShadowCone()), 1.f);
		
}