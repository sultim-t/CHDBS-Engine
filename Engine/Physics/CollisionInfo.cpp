#include "CollisionInfo.h"

CollisionInfo::CollisionInfo(CollisionType type) :
	Type(type),
	CollThis(nullptr),
	CollOther(nullptr),
	RbThis(nullptr),
	RbOther(nullptr) {}

// Copy collision info from broad phase
CollisionInfo::CollisionInfo(const BroadCollisionInfo &info)
{
	Type = info.Type;
	CollThis = info.CollThis;
	CollOther = info.CollOther;
	RbThis = info.RbThis;
	RbOther = info.RbOther;
}

float CollisionInfo::GetRestitution() const
{
	float a = CollThis->GetPhysicMaterial().GetRestitution();
	float b = CollOther->GetPhysicMaterial().GetRestitution();

	switch (CollThis->GetPhysicMaterial().GetRestitutionCombine())
	{
	case PhysicMaterialCombine::Average:
		return (a + b) * 0.5f;
	case PhysicMaterialCombine::Maximum:
		return Max(a, b);
	case PhysicMaterialCombine::Minimum:
		return Min(a, b);
	case PhysicMaterialCombine::Multiply:
		return a * b;
	default:
		ASSERT(0);
	}

	return 0.0f;
}

float CollisionInfo::GetStaticFriction() const
{
	float a = CollThis->GetPhysicMaterial().GetStaticFriction();
	float b = CollOther->GetPhysicMaterial().GetStaticFriction();

	switch (CollThis->GetPhysicMaterial().GetFrictionCombine())
	{
	case PhysicMaterialCombine::Average:
		return (a + b) * 0.5f;
	case PhysicMaterialCombine::Maximum:
		return Max(a, b);
	case PhysicMaterialCombine::Minimum:
		return Min(a, b);
	case PhysicMaterialCombine::Multiply:
		return a * b;
	default:
		ASSERT(0);
	}

	return 0.0f;
}


float CollisionInfo::GetDynamicFriction() const
{
	float a = CollThis->GetPhysicMaterial().GetDynamicFriction();
	float b = CollOther->GetPhysicMaterial().GetDynamicFriction();

	switch (CollThis->GetPhysicMaterial().GetFrictionCombine())
	{
	case PhysicMaterialCombine::Average:
		return (a + b) * 0.5f;
	case PhysicMaterialCombine::Maximum:
		return Max(a, b);
	case PhysicMaterialCombine::Minimum:
		return Min(a, b);
	case PhysicMaterialCombine::Multiply:
		return a * b;
	default:
		ASSERT(0);
	}

	return 0.0f;
}