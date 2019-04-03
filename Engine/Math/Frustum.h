#pragma once
#include "Plane.h"
#include "Matrix.h"

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
	// normals of planes directed to the outside of frustum
	Plane planes[FRUSTUM_PLANES_COUNT];
	
	Vector3 nearVerts[4];	// counter clockwise, starting from upper right
	Vector3 farVerts[4];	// counter clockwise, starting from upper right

	float fov;
	float aspect;			// width/height
	float near, far;

public:
	// Empty constructor
	Frustum();

	// Sets properties and calculates planes, vertices
	// "fov" is in Degrees
	// "aspect" is width/height
	void Init(float fov, float aspect, float near, float far);

	// Sets properties and calculates planes, vertices
	// "fov" is in Degrees
	// "aspect" is width/height
	// "transform" is matrix to transform vertices of frustum
	void Init(float fov, float aspect, float near, float far, const Matrix4 &transform);

	// Get field of view in Degrees
	inline float GetFOV() const;
	inline float GetAspect() const;
	inline float GetNearClipDist() const;
	inline float GetFarClipDist() const;
	// Get plane
	// Note: plane's normal directed to the outside of frustum
	inline const Plane &GetPlane(FrustumPlane side) const;
	// Get plane
	// Note: plane's normal directed to the outside of frustum
	inline const Plane &GetPlane(int side) const;

	// Sets and recalculates planes and vertices
	// "fov" is in degrees
	inline void SetFOV(float fov);
	// Sets and recalculates planes and vertices
	inline void SetAspect(float aspect);
	// Sets and recalculates planes and vertices
	inline void SetNearClipDist(float near);
	// Sets and recalculates planes and vertices
	inline void SetFarClipDist(float far);
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

inline const Plane &Frustum::GetPlane(int side) const
{
	return planes[side];
}

inline void Frustum::SetFOV(float fov)
{
	Init(fov, aspect, near, far);
}

inline void Frustum::SetAspect(float aspect)
{
	Init(fov, aspect, near, far);
}

inline void Frustum::SetNearClipDist(float near)
{
	Init(fov, aspect, near, far);
}

inline void Frustum::SetFarClipDist(float far)
{
	Init(fov, aspect, near, far);
}