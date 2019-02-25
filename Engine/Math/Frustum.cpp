#include "Frustum.h"

Frustum::Frustum()
{ }

void Frustum::Init(float fov, float aspect, float near, float far)
{
	this->fov = fov;
	this->aspect = aspect;
	this->near = near;
	this->far = far;

	float tanfov = Tan(fov / 0.5f);

	// Vector3 nearRight = (near * tanfov) * aspect *
	// ...

	//planes[(int)FrustumPlane::Near] = Plane(nearVerts[2], nearVerts[1], nearVerts[0]);
	//planes[(int)FrustumPlane::Near] = Plane(farVerts[0], farVerts[1], farVerts[2]);
}
