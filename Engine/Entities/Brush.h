#pragma once

#include <Engine/Math/Transform.h>
#include "Entity.h"

// Static Entity
class Brush : Entity
{
private:
	Matrix4 transformMat;
	bool transformInitialized = false;
public:
	Matrix4 GetStaticTransformMatrix()
	{
		if (transformInitialized)
		{
			return transformMat;
		}

		transformMat = transform.GetTransformMatrix();
		transformInitialized = true;

		return transformMat;
	}
};