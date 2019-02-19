#pragma once
#include "IComponent.h"
#include <Engine/Entities/Entity.h>

enum CameraProjection
{
	CAMERA_PROJ_PERSPECTIVE = 0,
	CAMERA_PROJ_ORTHOGRAPHIC = 1
};

class CCamera : public IComponent
{
	CLASSDECLARATION(CCamera)

private:
	CameraProjection projection;
	float zNear, zFar;

	// for perspective
	float fov;

	// for ortho
	float orthoLeft;
	float orthoRight;
	float orthoUp;
	float orthoBottom;

public:
	// Calculates view matrix
	Matrix4 GetViewMatrix() const;
	// Calculates projection matrix
	Matrix4 GetProjectionMatrix(float scrWidth, float scrHeight) const;

	inline CameraProjection GetProjection() const;
	inline float GetFOV() const;
	inline float GetNearClipDist() const;
	inline float GetFarClipDist() const;
	inline const Vector3 &GetPosition() const;

	inline void SetProjection(CameraProjection p);
	inline void SetFOV(float fov);
	inline void SetNearClipDist(float znear);
	inline void SetFarClipDist(float zfar);

	void Init() override;
	void Update() override {}
	
	void SetProperty(const String &key, const String &value) override;
};

inline CameraProjection CCamera::GetProjection() const
{
	return projection;
}

inline float CCamera::GetFOV() const
{
	return fov;
}

inline float CCamera::GetNearClipDist() const
{
	return zNear;
}

inline float CCamera::GetFarClipDist() const
{
	return zFar;
}

inline const Vector3 &CCamera::GetPosition() const
{
	ASSERT(owner != nullptr);

	return owner->GetTransform().GetPosition();
}

inline void CCamera::SetProjection(CameraProjection p)
{
	this->projection = p;
}

inline void CCamera::SetFOV(float fov)
{
	ASSERT(fov > 0);
	this->fov = fov;
}

inline void CCamera::SetNearClipDist(float zNear)
{
	ASSERT(zNear > 0);
	this->zNear = zNear;
}

inline void CCamera::SetFarClipDist(float zFar)
{
	ASSERT(zFar > 0);
	this->zFar = zFar;
}