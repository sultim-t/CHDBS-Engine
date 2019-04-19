#pragma once
#include "IComponent.h"
#include <Engine/Rendering/ICamera.h>
#include <Engine/Math/Frustum.h>

class CCamera : public IComponent, public ICamera
{
	CLASSDECLARATION(CCamera)

private:
	// Projection to use
	CameraProjection projection;
	
	// Distances to near and far planes
	float zNear, zFar;
	// Width / height
	float aspect;

	// For perspective projection
	float fov;

	// For orthographic projection
	float orthoLeft;
	float orthoRight;
	float orthoUp;
	float orthoBottom;

	// Frustum of this camera. Used for culling
	// Frustum frustum;

public:
	// Calculates view matrix
	Matrix4 GetViewMatrix() const override;
	// Calculates projection matrix
	Matrix4 GetProjectionMatrix() const override;
	// Get camera's view frustum in world space
	Frustum GetFrustum() const override;
	// Get camera's view frustum in world space
	// but with changed near and far planes
	Frustum GetFrustum(float newNear, float newFar) const override;

	CameraProjection GetProjection() const;
	float GetFOV() const;
	float GetNearClipDist() const;
	float GetFarClipDist() const;
	float GetAspect() const;
	const Vector3 &GetPosition() const;

	void SetProjection(CameraProjection p);
	void SetFOV(float fov);
	void SetNearClipDist(float znear);
	void SetFarClipDist(float zfar);
	void SetAspect(float width, float height);

	void Init() override;
	void Update() override {}
	
	void SetProperty(const String &key, const String &value) override;
};