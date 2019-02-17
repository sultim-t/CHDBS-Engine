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

bool Frustum::IsInside(const Vector3 &point) const
{
	for (int i = 0; i < FRUSTUM_PLANES_COUNT; i++)
	{
		if (!planes[i].IsInside(point))
		{
			return false;
		}
	}

	return true;
}

bool Frustum::IsInside(const Sphere & sphere) const
{
	return IsInside(sphere.GetCenter(), sphere.GetRadius());
}

bool Frustum::IsInside(const Vector3 & center, float radius) const
{
	for (int i = 0; i < FRUSTUM_PLANES_COUNT; i++)
	{
		if (!planes[i].IsInside(center, radius))
		{
			return false;
		}
	}

	return true;
}

void Frustum::SetFOV(float fov)
{
	Init(fov, aspect, near, far);
}

void Frustum::SetAspect(float aspect)
{
	Init(fov, aspect, near, far);
}

void Frustum::SetNearClipDist(float near)
{
	Init(fov, aspect, near, far);
}

void Frustum::SetFarClipDist(float far)
{
	Init(fov, aspect, near, far);
}