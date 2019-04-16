#include "MTransform.h"
#include "Quaternion.h"

void MTransform::SetIdentity()
{
	transform = Matrix4(1.0f, true);
}

Vector3 MTransform::DecomposePosition() const
{
	return Vector3(transform(3, 0), transform(3, 1), transform(3, 2));
}

Quaternion MTransform::DecomposeRotation() const
{
	// decompose scale
	float s[3];
	s[0] = Vector3(transform(0, 0), transform(0, 1), transform(0, 2)).Length();
	s[1] = Vector3(transform(1, 0), transform(1, 1), transform(1, 2)).Length();
	s[2] = Vector3(transform(2, 0), transform(2, 1), transform(2, 2)).Length();

	// decompose rotaion matrix
	Matrix3 rotationMatrix;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			rotationMatrix(i, j) = transform(i, j) / s[i];
		}
	}

	// convert to quaternion
	return Quaternion(rotationMatrix);
}

Vector3 MTransform::DecomposeScale() const
{
	return Vector3(
		Vector3(transform(0, 0), transform(0, 1), transform(0, 2)).Length(),
		Vector3(transform(1, 0), transform(1, 1), transform(1, 2)).Length(),
		Vector3(transform(2, 0), transform(2, 1), transform(2, 2)).Length());
}
