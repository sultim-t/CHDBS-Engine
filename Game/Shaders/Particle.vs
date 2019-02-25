#version 330 core

layout(location = 0) in vec3 vertices;
layout(location = 1) in vec4 xyzs;
layout(location = 2) in vec4 color;

out vec4 particlecolor;

uniform vec3 cameraRight;
uniform vec3 cameraUp;
uniform mat4 viewProj;

void main()
{
	float particleSize = xyzs.w;
	vec3 particlePos = xyzs.xyz;
	
	vec3 vertexPosWorld = 
		particlePos
		+ cameraRight * vertices.x * particleSize
		+ cameraUp * vertices.y * particleSize;

	gl_Position = viewProj * vec4(vertexPosWorld, 1.0f);

	particlecolor = color;
}

