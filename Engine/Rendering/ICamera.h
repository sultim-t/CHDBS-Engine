#pragma once

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
	// but with changed near and far vertices
	// "nearMult" is a multiplier in [0,1] : result frustum's near plane distance is OldNear+((OldFar-OldNear)*nearMult)
	// "farMult" is a multiplier in [0,1] : result frustum's far plane distance is OldNear+((OldFar-OldNear)*farMult)
	virtual Frustum GetFrustum(float nearMult, float farMult) const = 0;
};