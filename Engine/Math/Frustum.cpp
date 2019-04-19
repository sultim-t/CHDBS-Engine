#include "Frustum.h"

Frustum::Frustum()
{ }

void Frustum::Init(float fov, float aspect, float near, float far)
{
	// init from default tranformation
	Init(fov, aspect, near, far, Transform());
}

void Frustum::Init(float fovy, float aspect, float near, float far, const Transform &t)
{
	ASSERT(fovy > 0.0f && aspect > 0.0f && near > 0.0f && far > 0.0f);

	this->fov = fovy;
	this->aspect = aspect;
	this->near = near;
	this->far = far;
	float tanfov = Tan(DEG2RAD(fovy) * 0.5f);

	// half widths, heights
	float nearHHeight = tanfov * near;
	float nearHWidth = nearHHeight * aspect;
	float farHHeight = tanfov * far;
	float farHWidth = farHHeight * aspect;

	// from upper right, counter clockwise
	nearVerts[0] = t.PointFromLocal(Vector3(nearHWidth, nearHHeight, near));
	nearVerts[1] = t.PointFromLocal(Vector3(-nearHWidth, nearHHeight, near));
	nearVerts[2] = t.PointFromLocal(Vector3(-nearHWidth, -nearHHeight, near));
	nearVerts[3] = t.PointFromLocal(Vector3(nearHWidth, -nearHHeight, near));

	// from upper right, counter clockwise
	farVerts[0] = t.PointFromLocal(Vector3(farHWidth, farHHeight, far));
	farVerts[1] = t.PointFromLocal(Vector3(-farHWidth, farHHeight, far));
	farVerts[2] = t.PointFromLocal(Vector3(-farHWidth, -farHHeight, far));
	farVerts[3] = t.PointFromLocal(Vector3(farHWidth, -farHHeight, far));

	// clockwise, to make planes' normals point to the outside
	planes[(int)FrustumPlane::Near].FromPoints(nearVerts[0], nearVerts[1], nearVerts[2]);
	planes[(int)FrustumPlane::Far].FromPoints(farVerts[2], farVerts[1], farVerts[0]);

	planes[(int)FrustumPlane::Top].FromPoints(nearVerts[1], nearVerts[0], farVerts[0]);
	planes[(int)FrustumPlane::Bottom].FromPoints(nearVerts[3], nearVerts[2], farVerts[2]);

	planes[(int)FrustumPlane::Left].FromPoints(nearVerts[2], nearVerts[1], farVerts[1]);
	planes[(int)FrustumPlane::Right].FromPoints(farVerts[3], farVerts[0], nearVerts[0]);
}

float Frustum::GetFOV() const
{
	return fov;
}

float Frustum::GetAspect() const
{
	return aspect;
}

float Frustum::GetNearClipDist() const
{
	return near;
}

float Frustum::GetFarClipDist() const
{
	return far;
}

const Plane &Frustum::GetPlane(FrustumPlane side) const
{
	return planes[(int)side];
}

const Plane &Frustum::GetPlane(int side) const
{
	return planes[side];
}

const Vector3 * Frustum::GetNearVerts() const
{
	return nearVerts;
}

const Vector3 * Frustum::GetFarVerts() const
{
	return farVerts;
}

Vector3 Frustum::GetCenter() const
{
	Vector3 result = 0;

	for (int i = 0; i < 4; i++)
	{
		result += nearVerts[i];
	}

	for (int i = 0; i < 4; i++)
	{
		result += farVerts[i];
	}

	return result / 8.0f;
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