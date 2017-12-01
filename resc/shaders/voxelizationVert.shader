#version 450 core

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_texture_coordinates;

out vec3 geomNormal;
out vec3 geomPos;
out vec2 geomTexCoords;

uniform mat4 transform;
uniform mat4 model;

void main()
{
	geomPos = vec3(model * vec4(vertex_position, 1.0f));
	geomNormal = mat3( transpose( inverse( model ) ) ) * vertex_normal;
	geomTexCoords = vertex_texture_coordinates;
	gl_Position = transform * vec4(vertex_position.xyz, 1.0);
}