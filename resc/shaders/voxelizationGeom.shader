// Inspiration from https://github.com/Friduric/voxel-cone-tracing - Fredrik Präntare

#version 450 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 geomPos[];
in vec3 geomNormal[];
in vec2 geomTexCoords[];

out vec3 fragPos;
out vec3 fragNormal;
out vec2 fragTexCoords;

void main() {
	//Find normal of primitive
	const vec3 prNorm = abs(cross(geomPos[1] - geomPos[0], geomPos[2] - geomPos[0])); // abs(cross(ab,ac)).
	for (uint i = 0; i < 3; ++i) 
	{
		// As we only project along one axis, we move all geometry to the xy plane with the components that covers the most fragments.
		// Coords not used, only to invoke right amount of fragment shader calls
		if (prNorm.z > prNorm.x && prNorm.z > prNorm.y)
			gl_Position = vec4(geomPos[i].x, geomPos[i].y, 0, 1);
		else if (prNorm.x > prNorm.y && prNorm.x > prNorm.z)
			gl_Position = vec4(geomPos[i].y, geomPos[i].z, 0, 1);
		else
			gl_Position = vec4(geomPos[i].x, geomPos[i].z, 0, 1);
		// Send actual used position
		fragPos = geomPos[i];
		fragNormal = geomNormal[i];
		fragTexCoords = geomTexCoords[i];
		EmitVertex();
	}
	EndPrimitive();
}