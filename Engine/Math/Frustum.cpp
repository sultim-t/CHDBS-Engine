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

	float tanfov = Tan(fov / 0.5f);

	// half widths, heights
	float nearHWidth = tanfov * near;
	float nearHHeight = aspect * nearHWidth;
	float farHWidth = tanfov * far;
	float farHHeight = aspect * farHWidth;

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
	planes[(int)FrustumPlane::Near].FromPoints(nearVerts[2], nearVerts[1], nearVerts[0]);
	planes[(int)FrustumPlane::Far].FromPoints(farVerts[2], farVerts[1], farVerts[0]);

	planes[(int)FrustumPlane::Bottom].FromPoints(nearVerts[3], nearVerts[2], farVerts[2]);
	planes[(int)FrustumPlane::Top].FromPoints(nearVerts[0], nearVerts[1], farVerts[1]);

	planes[(int)FrustumPlane::Right].FromPoints(nearVerts[3], nearVerts[0], farVerts[0]);
	planes[(int)FrustumPlane::Left].FromPoints(nearVerts[1], nearVerts[2], farVerts[2]);
}
