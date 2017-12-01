#version 450 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_texture_coordinates;

out vec3 fragNorm;
out vec3 fragPos;
out vec2 texCoords;

uniform mat4 transform;
uniform mat4 model;

uniform sampler3D voxGrid;

uniform int gridSize;

#define AORANGE 0.1

float calcAO(vec3 vertPos, vec3 norm)
{
	float adjustedSize = gridSize;
	float voxelSize = 1.f / adjustedSize;
	float normRange = AORANGE * adjustedSize;

	float aoAcc = 0; // accumulation of AO
	for (int ix = int(-normRange); ix <= normRange; ++ix)
	{
		for (int iy = int(-normRange); iy <= normRange; ++iy)
		{
			for (int iz = int(-normRange); iz <= normRange; ++iz)
			{
				vec3 loopVoxVec = vertPos + voxelSize * vec3(ix, iy, iz);
				vec4 texColor = textureLod(voxGrid, 0.5f*(loopVoxVec)+vec3(0.5f), 0.f);
				if (texColor != vec4(0))
				{
					vec3 fragToVox = loopVoxVec - vertPos;
					aoAcc += 2.f * max(dot(normalize(fragToVox), normalize(norm)), 0.f) * max(AORANGE - length(fragToVox), 0.f)/pow(1.f + 15.f * length(fragToVox), 5.f);
				}
			}
		}
	}

	return aoAcc;
}

void main()
{
	fragNorm = mat3(transpose(inverse(model)))*vertex_normal;
	gl_Position = transform*vec4(vertex_position.xyz, 1.0);
	fragPos = vec3(model*vec4(vertex_position, 1.0f));
	texCoords = vertex_texture_coordinates;
}