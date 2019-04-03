#include "DebugDrawer.h"
#include "OpenGL.h"
#include <Engine/Math/AABB.h>
#include <Engine/Math/Sphere.h>
#include <Engine/Math/Frustum.h>

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
	for (int i = 0; i < lines.GetTop(); i++)
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

void DebugDrawer::Draw(const Sphere & sphere, const Color4 & color)
{
	const Vector3 &center = sphere.GetCenter();
	float radius = sphere.GetRadius();

	const int step = 30;
	const int count = 360 / step;
	Vector3 cache[count];
	
	cache[0] = center + Vector3(0.0f, radius, 0.0f);

	for (int i = 0; i < count; i++)
	{
		cache[i] = cache[0];
	}

	Vector3 lastPoint, temp;

	for (int i = step; i <= 360; i += step)
	{
		float s = Sin(DEG2RAD(i));
		float c = Cos(DEG2RAD(i));

		lastPoint[0] = center[0];
		lastPoint[2] = center[2] + radius * s;
		lastPoint[1] = center[1] + radius * c;

		for (int k = 0, j = step; j <= 360; j += step, k++)
		{
			temp[0] = center[0] + Sin(DEG2RAD(j)) * radius * s;
			temp[2] = center[2] + Cos(DEG2RAD(j)) * radius * s;
			temp[1] = lastPoint[1];

			DebugDrawLine line1(lastPoint, temp, color);
			DebugDrawLine line2(lastPoint, cache[k], color);
			lines.Push(line1);
			lines.Push(line2);

			cache[k] = lastPoint;
			lastPoint = temp;
		}
	}
}

void DebugDrawer::Draw(const AABB & aabb, const Color4 & color)
{
	Vector3 min = aabb.GetMin();
	Vector3 max = aabb.GetMax();
	
	Vector3 points[8];

	// create 4 bottom points
	points[0] = min;
	
	points[1] = min;
	points[1][0] = max[0];

	points[2] = min;
	points[2][0] = max[0];
	points[2][2] = max[2];

	points[3] = min;
	points[3][2] = max[2];

	// create 4 up points
	// offsetted along vertical axis
	for (int i = 4; i < 8; i++)
	{
		points[i] = points[i - 4];
		points[i][1] = max[1];
	}

	// add lines
}

void DebugDrawer::Draw(const Frustum & frustum, const Color4 & color)
{
	const Vector3 *near = frustum.GetNearVerts();
	const Vector3 *far = frustum.GetFarVerts();

	Vector3 points[8];
	points[0] = near[0];
	points[1] = near[1];
	points[2] = near[2];
	points[3] = near[3];
	points[4] = far[0];
	points[5] = far[1];
	points[6] = far[2];
	points[7] = far[3];

	DrawBox(points, color);
}

void DebugDrawer::DrawBox(const Vector3 * points, const Color4 & color)
{
	// Vertices should be indexed like that:
	//
	// 7------6
	// | \    | \
	// |  4---+--5
	// |  |   |  |
	// 3--+---2  |
	//   \|     \|
	//    0------1
	// 

	for (int i = 0; i < 4; i++)
	{
		DebugDrawLine line1(points[i], points[(i + 1) % 4], color);
		DebugDrawLine line2(points[i], points[i + 4], color);
		DebugDrawLine line3(points[i + 4], points[((i + 1) % 4) + 4], color);
		lines.Push(line1);
		lines.Push(line2);
		lines.Push(line3);
	}
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
