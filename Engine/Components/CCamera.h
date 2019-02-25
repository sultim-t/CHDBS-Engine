#pragma once
#include "IComponent.h"
#include <Engine/Entities/Entity.h>

enum class CameraProjection
{
	Perspective,
	Orthographic
};

class CCamera : public IComponent
{
	CLASSDECLARATION(CCamera)

private:
	CameraProjection projection;
	float zNear, zFar;
	float aspect; // width / height

	// for perspective
	float fov;

	// for ortho
	float orthoLeft;
	float orthoRight;
	float orthoUp;
	float orthoBottom;

	// TODO: add frustum
	// change frustum's fov
	// - only if camera's fov changed >3 degrees
	// - change by step = 3 degrees
	// make sure that frustum's fov > camera's fov

public:
	// Calculates view matrix
	Matrix4 GetViewMatrix() const;
	// Calculates projection matrix
	Matrix4 GetProjectionMatrix() const;

	inline CameraProjection GetProjection() const;
	inline float GetFOV() const;
	inline float GetNearClipDist() const;
	inline float GetFarClipDist() const;
	inline float GetAspect() const;
	inline const Vector3 &GetPosition() const;

	inline void SetProjection(CameraProjection p);
	inline void SetFOV(float fov);
	inline void SetNearClipDist(float znear);
	inline void SetFarClipDist(float zfar);
	inline void SetAspect(float width, float height);

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

inline float CCamera::GetAspect() const
{
	return aspect;
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

inline void CCamera::SetAspect(float width, float height)
{
	aspect = width / height;
}
