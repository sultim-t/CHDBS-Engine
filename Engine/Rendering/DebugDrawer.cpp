#include "DebugDrawer.h"
#include "OpenGL.h"

#define SHADER_MVP_MATRIX_NAME "u_MVP"

void DebugDrawer::InitBuffers()
{
	glGenVertexArrays(1, &linesVAO);
	glGenBuffers(1, &linesVBO);

	glBindVertexArray(linesVAO);
	glBindBuffer(GL_ARRAY_BUFFER, linesVBO);

	glBufferData(GL_ARRAY_BUFFER, DEBUG_DRAWER_MAX_VERTS * sizeof(DebugDrawVertex), nullptr, GL_STREAM_DRAW);

	// position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DebugDrawVertex), (void*)0);

	// color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(DebugDrawVertex), (void*)(sizeof(Vector3)));

	// unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void DebugDrawer::InitShader(const char *vertPath, const char *fragPath)
{
	debugShader.Load(vertPath, fragPath);
}

void DebugDrawer::Init(const char *vertPath, const char *fragPath)
{
	InitBuffers();
	InitShader(vertPath, fragPath);

	// init buffer
	vertices.Init(DEBUG_DRAWER_MAX_VERTS);
	lines.Init(DEBUG_DRAWER_MAX_LINES);
}

void DebugDrawer::BindSpaceMatrix(const Matrix4 & space)
{
	this->space = space;
}

void DebugDrawer::DrawQueues()
{
	// process lines
	for (UINT i = 0; i < lines.GetTop(); i++)
	{
		// extract verteices
		DebugDrawVertex v0(lines[i].From, lines[i].Color);
		DebugDrawVertex v1(lines[i].To, lines[i].Color);

		// push them
		vertices.Push(v0);
		vertices.Push(v1);
	}
	// draw them
	DrawAllLines();

	// clear all buffers
	lines.Clear();
	vertices.Clear();
}

void DebugDrawer::DrawAllLines()
{
	// get vertices count
	UINT count = vertices.GetTop();

	glBindVertexArray(linesVAO);

	debugShader.Use();
	debugShader.SetMat4(SHADER_MVP_MATRIX_NAME, space);

	// bind buffer
	glBindBuffer(GL_ARRAY_BUFFER, linesVBO);
	// add vertices, which were created from lines' start and end points
	glBufferSubData(GL_ARRAY_BUFFER, 0, count * sizeof(DebugDrawVertex), vertices.GetArray());

	// draw call
	glDrawArrays(GL_LINES, 0, count);

	// unbind
	debugShader.Stop();
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void DebugDrawer::Draw(const Sphere & sphere, const Vector3 & position, const Color4 & color)
{
	// todo
}

void DebugDrawer::Draw(const AABB & aabb, const Vector3 & position, const Color4 & color)
{
	// todo
}

void DebugDrawer::Draw(const Vector3 & a, const Vector3 & b, const Color4 & color)
{
	// create and add line to render queue
	DebugDrawLine line(a, b, color);
	lines.Push(line);
}

DebugDrawer & DebugDrawer::Instance()
{
	static DebugDrawer instance;
	return instance;
}
