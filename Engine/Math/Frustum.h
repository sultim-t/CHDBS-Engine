#pragma once
#include "Plane.h"

#define FRUSTUM_PLANES_COUNT 6

class Frustum
{
public:
	enum class FrustumPlane
	{
		Near, Far,
		Left, Right,
		Top, Bottom
	};

private:
	Plane planes[FRUSTUM_PLANES_COUNT];
	
	Vector3 nearVerts[4];
	Vector3 farVerts[4];

	float fov;
	float aspect;
	float near, far;

public:
	// Empty constructor
	Frustum();

	// Sets properties and calculates planes, vertices
	// "fov" is in degrees
	// "aspect" is width/height
	void Init(float fov, float aspect, float near, float far);

	bool IsInside(const Vector3 &point) const;
	bool IsInside(const Sphere &sphere) const;
	bool IsInside(const Vector3 &center, float radius) const;

	// Get field of view in degrees
	inline float GetFOV() const;
	inline float GetAspect() const;
	inline float GetNearClipDist() const;
	inline float GetFarClipDist() const;
	inline const Plane &GetPlane(FrustumPlane side) const;

	// Sets and recalculates planes and vertices
	// "fov" is in degrees
	void SetFOV(float fov);
	// Sets and recalculates planes and vertices
	void SetAspect(float aspect);
	// Sets and recalculates planes and vertices
	void SetNearClipDist(float near);
	// Sets and recalculates planes and vertices
	void SetFarClipDist(float far);
};

inline float Frustum::GetFOV() const
{
	return fov;
}

inline float Frustum::GetAspect() const
{
	return aspect;
}

inline float Frustum::GetNearClipDist() const
{
	return near;
}

inline float Frustum::GetFarClipDist() const
{
	return far;
}

inline const Plane &Frustum::GetPlane(FrustumPlane side) const
{
	return planes[(int)side];
}