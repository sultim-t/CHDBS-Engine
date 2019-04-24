#pragma once
#include <Engine/Math/Vector.h>

struct RaycastInfo
{
	// Hit point
	Vector3 Point;
	// Normal of hit point
	Vector3 Normal;
	// Distance from start to hitted point
	float Distance;
	// Hitted collider (mustn't be null)
	const ICollider *Hitted;
	// Hitted rigidbody (can be null)
	const Rigidbody *HittedRigidbody;
};