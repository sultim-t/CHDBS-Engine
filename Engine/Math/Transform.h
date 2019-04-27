#pragma once

#include "Vector.h"
#include "Matrix.h"
#include "Quaternion.h"

class Transform
{
private:
	Vector3 position;
	Vector3 scale;
	Quaternion quat;

public:
	// Transform with default values
	Transform();
	Transform(const Vector3 &pos, const Vector3 &eulerAngles = (0.0f, 0.0f, 0.0f), const Vector3 &scale = (1.0f, 1.0f, 1.0f));
	Transform(const Vector3 &pos, const Quaternion &rot, const Vector3 &scale = (1.0f, 1.0f, 1.0f));
	Transform(const Matrix4 &m);

	// Translate transform
	void Translate(const Vector3 &vec);
	// Rotate with euler's angles
	void Rotate(const Vector3 &euler);
	// Rotate with quaternion
	void Rotate(const Quaternion &quat);
	// Scale by one multiplier
	void Scale(const float scale);
	// Scale each axis
	void Scale(const Vector3 &scale);

	// Resets position, rotation and scale
	void Reset();

	// Position vector
	const Vector3 &GetPosition() const;
	// Returns reference to position vector
	Vector3 &GetPosition();
	// Sets position
	void SetPosition(const Vector3 &position);

	// Calculate rotation represented in euler's angles
	const Vector3 GetEuler() const;
	// Sets euler's angles
	void SetRotation(const Vector3 &euler);

	// Scale vector
	const Vector3 &GetScale() const;
	// Returns reference to scale vector
	Vector3 &GetScale();
	// Sets scale
	void SetScale(const Vector3 &scale);

	// Rotation represented in quaternion
	const Quaternion &GetRotation() const;
	// Returns reference to quaternion
	Quaternion &GetRotation();
	// Sets quaternion
	void SetRotation(Quaternion &quat);

	// Get forward vector
	const Vector3 GetForward() const;
	// Get right vector 
	const Vector3 GetRight() const;
	// Get up vector
	const Vector3 GetUp() const;
	// Get forward, right and up vector
	const void GetFRU(Vector3 &f, Vector3 &r, Vector3 &u) const;

	// Calclate tranformation matrix
	const Matrix4 GetTransformMatrix() const;

	// Returns translated matrix
	static Matrix4 TranslateMatrix(const Matrix4& mat, const Vector3& vec);
	// Returns rotated matrix, angle in Radians
	static Matrix4 RotateMatrix(const Matrix4& mat, const  Vector3& axis, const float radians);
	// Returns rotated matrix, angle in Radians
	static Matrix4 RotateMatrix(const Matrix4& mat, const Quaternion &quat);
	// Returns scaled matrix
	static Matrix4 ScaleMatrix(const Matrix4& mat, const Vector3& vec);

	// Create LookAt matrix
	static Matrix4 LookAt(const Vector3 &position, const Vector3 &target, const Vector3 &up);

	// Decompose position from matrix
	static Vector3 DecomposePosition(const Matrix4 &m);
	// Decompose rotation from matrix
	static Quaternion DecomposeRotation(const Matrix4 &m);
	// Decompose scale from matrix
	static Vector3 DecomposeScale(const Matrix4 &m);

	// Decompose tranform from matrix
	// Engine is using transposed matrix, so "m" is too
	void FromMatrix(const Matrix4 &m);

	// Transform direction from local space to world
	// Returns dir in world space
	Vector3 DirectionFromLocal(const Vector3 &dir) const;
	// Transform direction from world space to local
	// Returns dir in local space
	Vector3 DirectionFromWorld(const Vector3 &dir) const;
	// Transform point from local space to world
	// Note: affected by scale
	Vector3 PointFromLocal(const Vector3 &vec) const;
	// Transform point from world space to local
	// Note: affected by scale
	Vector3 PointFromWorld(const Vector3 &vec) const;
};

