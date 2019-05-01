#pragma once

#include <Engine/Math/Matrix.h>
#include <Engine/DataStructures/StaticStack.h>
#include <Engine/DataStructures/DynamicArray.h>
#include <Engine/Rendering/Shader.h>

class DebugDrawer
{
private:
#define DEBUG_DRAWER_MAX_VERTS 4096
#define DEBUG_DRAWER_LINES_COUNT 32768

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
	const Shader *debugShader;

	// All vertices to draw
	DynamicArray<DebugDrawVertex> vertices;
	// All lines to draw
	DynamicArray<DebugDrawLine> lines;

	// Current space matrix, to pass to shader
	Matrix4 space;

private:
	void InitBuffers();

	// Draw box from 8 points
	void DrawBox(const Vector3 *points, const Color4 &color);
	// Draw all lines from buffer
	void DrawAllLines();

public:
	// Init buffers and shader
	void Init();
	// Set debugging shader
	void SetShader(const char *name);

	// Bind matrix to draw primitives
	void BindSpaceMatrix(const Matrix4 &space);
	// Actually draws all data from queues
	void DrawQueues();

	// Add sphere to render queue
	void Draw(const Sphere &sphere, const Color4 &color = Color4(0, 255, 0, 255));
	// Add AABB to render queue
	void Draw(const AABB &aabb, const Color4 &color = Color4(0, 255, 0, 255));
	// Add AABB with tranformation to render queue
	void Draw(const AABB &aabb, const Matrix4 &transform, const Color4 &color = Color4(0, 255, 0, 255));
	// Add frustum to render queue
	void Draw(const Frustum &frustum, const Color4 &color = Color4(0, 255, 0, 255));
	// Add line to render queue
	void Draw(const Vector3 &a, const Vector3 &b, const Color4 &color = Color4(0, 255, 0, 255));

	// Get instance
	static DebugDrawer &Instance();
};