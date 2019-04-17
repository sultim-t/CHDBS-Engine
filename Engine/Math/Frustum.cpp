#include "Frustum.h"

Frustum::Frustum()
{ }

void Frustum::Init(float fov, float aspect, float near, float far)
{
#if _DEBUG
	if (fov <= 0.0f || aspect <= 0.0f || near <= 0.0f || far <= 0.0f)
	{
		Logger::Print("Frustum::Init:: arguments are < 0");
	}
#endif

	this->fov = fov;
	this->aspect = aspect;
	this->near = near;
	this->far = far;

	float tanfov = Tan(DEG2RAD(fov) * 0.5f);

	// half widths, heights
	float nearHWidth = tanfov * near;
	float nearHHeight = nearHWidth / aspect;
	float farHWidth = tanfov * far;
	float farHHeight = farHWidth / aspect;

	// from upper right, counter clockwise
	nearVerts[0] = Vector3(nearHWidth, nearHHeight, near);
	nearVerts[1] = Vector3(-nearHWidth, nearHHeight, near);
	nearVerts[2] = Vector3(-nearHWidth, -nearHHeight, near);
	nearVerts[3] = Vector3(nearHWidth, -nearHHeight, near);

	// from upper right, counter clockwise
	farVerts[0] = Vector3(farHWidth, farHHeight, far);
	farVerts[1] = Vector3(-farHWidth, farHHeight, far);
	farVerts[2] = Vector3(-farHWidth, -farHHeight, far);
	farVerts[3] = Vector3(farHWidth, -farHHeight, far);

	// clockwise, to make planes' normals point to the outside
	planes[(int)FrustumPlane::Near].FromPoints(nearVerts[0], nearVerts[1], nearVerts[2]);
	planes[(int)FrustumPlane::Far].FromPoints(farVerts[2], farVerts[1], farVerts[0]);

	planes[(int)FrustumPlane::Top].FromPoints(nearVerts[1], nearVerts[0], farVerts[0]);
	planes[(int)FrustumPlane::Bottom].FromPoints(nearVerts[3], nearVerts[2], farVerts[2]);

	planes[(int)FrustumPlane::Left].FromPoints(nearVerts[2], nearVerts[1], farVerts[1]);
	planes[(int)FrustumPlane::Right].FromPoints(farVerts[3], farVerts[0], nearVerts[0]);
}

void Frustum::Init(float fov, float aspect, float near, float far, const Matrix4 &transformMatrix)
{
#if _DEBUG
	if (fov <= 0.0f || aspect <= 0.0f || near <= 0.0f || far <= 0.0f)
	{
		Logger::Print("Frustum::Init:: arguments are < 0");
	}
#endif

	this->fov = fov;
	this->aspect = aspect;
	this->near = near;
	this->far = far;
	float tanfov = Tan(DEG2RAD(fov) * 0.5f);

	// half widths, heights
	float nearHWidth = tanfov * near;
	float nearHHeight = nearHWidth / aspect;
	float farHWidth = tanfov * far;
	float farHHeight = farHWidth / aspect;

	// tranform vertices from local to gloval space
	// transpose matrix to calculate final vert position as Matrix*Vector
	Matrix4 transform = transformMatrix.GetTransposed();

	// from upper right, counter clockwise
	nearVerts[0] = transform * Vector4(nearHWidth, nearHHeight, near, 1.0f);
	nearVerts[1] = transform * Vector4(-nearHWidth, nearHHeight, near, 1.0f);
	nearVerts[2] = transform * Vector4(-nearHWidth, -nearHHeight, near, 1.0f);
	nearVerts[3] = transform * Vector4(nearHWidth, -nearHHeight, near, 1.0f);

	// from upper right, counter clockwise
	farVerts[0] = transform * Vector4(farHWidth, farHHeight, far, 1.0f);
	farVerts[1] = transform * Vector4(-farHWidth, farHHeight, far, 1.0f);
	farVerts[2] = transform * Vector4(-farHWidth, -farHHeight, far, 1.0f);
	farVerts[3] = transform * Vector4(farHWidth, -farHHeight, far, 1.0f);

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