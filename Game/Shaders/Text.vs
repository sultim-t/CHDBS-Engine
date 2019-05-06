#version 330 core

layout (location = 0) in vec4 Vertex;

out vec2 TexCoords;
uniform mat4 Projection;

void main()
{
	gl_Position = Projection * vec4(Vertex.xy, 0.0, 1.0);
	TexCoords = Vertex.zw;
}