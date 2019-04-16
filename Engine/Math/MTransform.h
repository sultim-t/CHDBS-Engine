#pragma once

#include "Matrix.h"

// Transformation matrix
// Engine is using transposed tranformation matrix, i.e.
// a b c 0
// d e f 0
// g h i 0
// j k l 1
class MTransform
{
private:
	Matrix4 transform;

public:

	// Set identity tranformation
	void SetIdentity();

	// Decompose position from matrix
	Vector3 DecomposePosition() const;
	// Decompose rotation from matrix
	Quaternion DecomposeRotation() const;
	// Decompose scale from matrix
	Vector3 DecomposeScale() const;

};