#include "Projection.h"
#include "Matrix.h"

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

Matrix4 Projection::Perspective(float fov, float aspect, float zNear, float zFar)
{
	glm::mat4 persp = glm::perspective(glm::radians(fov), aspect, zNear, zFar);

	Matrix4 result;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result(i, j) = persp[i][j];
		}
	}

	return result;
}

Matrix4 Projection::Ortho(float left, float right, float bottom, float up, float zNear, float zFar)
{
	glm::mat4 ortho = glm::ortho(left, right, bottom, up, zNear, zFar);

	Matrix4 result;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result(i, j) = ortho[i][j];
		}
	}

	return result;
}

/// Creates a matrix for a left handed, symetric perspective-view frustum.
/// The near and far clip planes correspond to z normalized device coordinates of -1 and +1 respectively. (OpenGL clip volume definition)
/*Matrix4 Projection::Perspective(float fov, float aspect, float zNear, float zFar)
{
	Matrix4 mat(0.0f);

	float ctgHalfFov = 1.0f / Tan(DEG2RAD(fov) * 0.5f);
	float invDiff = 1.0f / (zFar - zNear);

	mat(0, 0) = ctgHalfFov / aspect;
	mat(1, 1) = ctgHalfFov;
	mat(2, 2) = (zFar + zNear) * invDiff;
	mat(2, 3) = 1.0f;
	mat(3, 2) = -2.0f * zFar * zNear * invDiff;

	return mat;
}*/

/*Matrix4 Projection::Perspective(float fov, float aspect, float zNear, float zFar)
{
	Matrix4 mat(0.0f);

	float ctgHalfFov = 1.0f / Tan(DEG2RAD(fov) * 0.5f);
	float invDiff = 1.0f / (zFar - zNear);

	mat(0, 0) = ctgHalfFov;
	mat(1, 1) = ctgHalfFov * aspect;
	mat(2, 2) = -(zFar + zNear) * invDiff;
	mat(2, 3) = -1.0f;
	mat(3, 2) = -2.0f * zFar * zNear * invDiff;

	return mat;
}*/