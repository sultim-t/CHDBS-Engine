#include "Projection.h"
#include "Matrix.h"

Matrix4 Projection::Perspective(float fov, float aspect, float zNear, float zFar)
{
	Matrix4 mat(0.0f);

	float ctgHalfFov = 1.0f / Tan(DEG2RAD(fov * 0.5f));
	float invDiff = 1.0f / (zFar - zNear);

	mat(0, 0) = ctgHalfFov/ aspect;			// left: ctgHalfFov * aspect;
	mat(1, 1) = ctgHalfFov;					// left: ctgHalfFov;
	mat(2, 2) = (zFar + zNear) * invDiff;	// left: without minus
	mat(2, 3) = 1.0f; // left: 1.0f;
	mat(3, 2) = -2.0f * zFar * zNear * invDiff;

	return mat;
}

Matrix4 Projection::Ortho(float left, float right, float bottom, float up, float zNear, float zFar)
{
	Matrix4 mat = Matrix4(1.0f, true);

	mat(0, 0) = 2.0f / (right - left);
	mat(1, 1) = 2.0f / (up - bottom);
	mat(2, 2) = -2.0f / (zFar - zNear);
	mat(3, 0) = -(right + left) / (right - left);
	mat(3, 1) = -(up + bottom) / (up - bottom);
	mat(3, 2) = -(zFar + zNear) / (zFar - zNear);

	return mat;
}

