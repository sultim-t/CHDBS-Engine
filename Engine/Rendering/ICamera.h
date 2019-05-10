#pragma once
#include <Engine/Math/Transform.h>

enum class CameraProjection
{
	Perspective,
	Orthographic
};

class ICamera
{
public:
	// Calculates view matrix
	virtual Matrix4 GetViewMatrix() const = 0;
	// Calculates projection matrix
	virtual Matrix4 GetProjectionMatrix() const = 0;
	// Get camera's view frustum in world space
	virtual Frustum GetFrustum() const = 0;
	// Get camera's view frustum in world space
	// but with changed near and far planes
	virtual Frustum GetFrustum(float newNear, float newFar) const = 0;

	virtual void SetAspect(float width, float height) = 0;
	virtual const Transform &GetTransform() const = 0;
};