#pragma once

class Projection
{
private:
	Projection();
public:
	// Calculates perspective matrix
	//   "fov" is a field of view in degrees
	static Matrix4 Perspective(float fov, float aspect, float zNear, float zFar);
	static Matrix4 Ortho(float left, float right, float bottom, float up, float zNear, float zFar);
};