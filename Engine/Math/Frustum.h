#pragma once
#include "Plane.h"
#include "Transform.h"

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
	// "fovy" is in Degrees
	// "aspect" is width/height
	// "t" to transform vertices of frustum
	void Init(float fovy, float aspect, float near, float far, const Transform &t);

	// Get field of view in Degrees
	float GetFOV() const;
	float GetAspect() const;
	float GetNearClipDist() const;
	float GetFarClipDist() const;

	// Get plane
	// Note: plane's normal directed to the outside of frustum
	const Plane &GetPlane(FrustumPlane side) const;
	// Get plane
	// Note: plane's normal directed to the outside of frustum
	const Plane &GetPlane(int side) const;
	
	const Vector3 *GetNearVerts() const;
	const Vector3 *GetFarVerts() const;

	Vector3 GetCenter() const;

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