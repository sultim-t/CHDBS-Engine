#pragma once

#include <Engine/Math/AABB.h>
#include <Engine/Math/Sphere.h>
#include <Engine/Math/Matrix.h>
#include <Engine/DataStructures/StaticStack.h>
#include <Engine/Rendering/Shader.h>

class DebugDrawer
{
private:
#define DEBUG_DRAWER_MAX_VERTS 4096
#define DEBUG_DRAWER_MAX_LINES 32768

	struct DebugDrawVertex
	{
		Vector3 Position;
		Color4 Color;

		DebugDrawVertex(Vector3 pos, Color4 color)
			: Position(pos), Color(color) {}
	};

	struct DebugDrawLine
	{
		Vector3 From;
		Vector3 To;
		Color4 Color;

		DebugDrawLine(Vector3 from, Vector3 to, Color4 color)
			: From(from), To(to), Color(color) {}
	};

private:
	UINT linesVAO;
	UINT linesVBO;

	// Shader which is used for debugging
	Shader debugShader;

	// All vertices to draw
	StaticStack<DebugDrawVertex> vertices;
	// All lines to draw
	StaticStack<DebugDrawLine> lines;

	// Current space matrix, to pass to shader
	Matrix4 space;

private:
	void InitBuffers();
	void InitShader(const char *vertPath, const char *fragPath);

	// Draw all lines from buffer
	void DrawAllLines();

public:
	// Init buffers and shader
	void Init(const char *vertPath, const char *fragPath);

	// Bind matrix to draw primitives
	void BindSpaceMatrix(const Matrix4 &space);
	// Actually draws all data from queues
	void DrawQueues();

	// Add sphere to render queue
	void Draw(const Sphere &sphere, const Vector3 &position = Vector3(0.0f), const Color4 &color = Color4(0, 255, 0, 255));
	// Add AABB to render queue
	void Draw(const AABB &aabb, const Vector3 &position = Vector3(0.0f), const Color4 &color = Color4(0, 255, 0, 255));
	// Add line to render queue
	void Draw(const Vector3 &a, const Vector3 &b, const Color4 &color = Color4(0, 255, 0, 255));

	// Get instance
	static DebugDrawer &Instance();
};