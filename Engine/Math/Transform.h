#pragma once

#include "BasicMath.h"
#include "Vector.h"
#include "Matrix.h"
#include "Quaternion.h"

class Transform
{
private:
	Vector3 position;
	Vector3 scale;
	Quaternion quat;

	mutable Vector3 forward, right, up;
	mutable bool fruWasModified;

public:
	// Transform with default values
	Transform();
	Transform(const Vector3 &pos, const Vector3 &eulerAngles = (0.0f, 0.0f, 0.0f), const Vector3 &scale = (1.0f, 1.0f, 1.0f));
	Transform(const Vector3 &pos, const Quaternion &rot, const Vector3 &scale = (1.0f, 1.0f, 1.0f));

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
	const void GetPosition(Vector3 &position) const;
	// Copy of position vector
	const Vector3 GetPosition() const;
	// Sets position
	void SetPosition(Vector3 &position);

	// Rotation represented in euler's angles
	const void GetEuler(Vector3 &euler) const;
	// Copy of rotation vector
	const Vector3 GetEuler() const;
	// Sets euler's angles
	void SetRotation(Vector3 &euler);

	// Scale vector
	const void GetScale(Vector3 &scale) const;
	// Copy of scale vector
	const Vector3 GetScale() const;
	// Sets scale
	void SetScale(Vector3 &scale);

	// Rotation represented in quaternion
	const void GetRotation(Quaternion &quat) const;
	// Copy of quaternion
	const Quaternion GetRotation() const;
	// Sets quaternion
	void SetRotation(Quaternion &quat);

	// Get forward vector
	const Vector3 GetForward() const;
	// Get world right vector
	// static Vector3 GetRight(Vector3 forward);
	// Get world right vector 
	const Vector3 GetRight() const;
	// Get up vector
	const Vector3 GetUp() const;
	// Get forward, right and up vector
	const void GetFRU(Vector3 &f, Vector3 &r, Vector3 &u) const;

	// Returns tranformation matrix
	const Matrix4 GetTransformMatrix() const;

	// Returns translated matrix
	static Matrix4 TranslateMatrix(const Matrix4& mat, const Vector3& vec);
	// Returns rotated matrix, angle in Radians
	static Matrix4 RotateMatrix(const Matrix4& mat, const  Vector3& axis, const float radians);
	// Returns scaled matrix
	static Matrix4 ScaleMatrix(const Matrix4& mat, const Vector3& vec);

	static Matrix4 LookAt(const Vector3 &position, const Vector3 &target, const Vector3 &up);
};

