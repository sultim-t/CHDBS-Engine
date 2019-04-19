#include "CCollider.h"

#define PROPERTY_KEY_COLSTATICFRICTION	"StaticFriction"
#define PROPERTY_KEY_COLDYNAMICFRICTION	"DynamicFriction"
#define PROPERTY_KEY_COLFRICTCOMBINE	"FrictionCombine"

#define PROPERTY_KEY_COLRESTITUTION		"Restitution"
#define PROPERTY_KEY_COLRESTITCOMBINE	"RestitutionCombine"

#define PROPERTY_KEY_COLCOMBINEAVERAGE	"CombineAverage"
#define PROPERTY_KEY_COLCOMBINEMIN		"CombineMinimum"
#define PROPERTY_KEY_COLCOMBINEMAX		"CombineMaximum"
#define PROPERTY_KEY_COLCOMBINEMULTIPLY	"CombineMultiply"

bool CCollider::SetProperty(ICollider *collider, const String &key, const String &value)
{
	if (key == PROPERTY_KEY_COLSTATICFRICTION)
	{
		ASSERT(collider != nullptr);
		collider->GetPhysicMaterial().SetStaticFriction(value.ToFloat());

		return true;
	}
	else if (key == PROPERTY_KEY_COLDYNAMICFRICTION)
	{
		ASSERT(collider != nullptr);
		collider->GetPhysicMaterial().SetDynamicFriction(value.ToFloat());

		return true;
	}
	else if (key == PROPERTY_KEY_COLFRICTCOMBINE)
	{
		ASSERT(collider != nullptr);
		collider->GetPhysicMaterial().SetFrictionCombine(GetCombineOption(value));

		return true;
	}
	else if (key == PROPERTY_KEY_COLRESTITUTION)
	{
		ASSERT(collider != nullptr);
		collider->GetPhysicMaterial().SetRestitution(value.ToFloat());

		return true;
	}
	else if (key == PROPERTY_KEY_COLRESTITCOMBINE)
	{
		ASSERT(collider != nullptr);
		collider->GetPhysicMaterial().SetRestitutionCombine(GetCombineOption(value));

		return true;
	}

	return false;
}

PhysicMaterialCombine CCollider::GetCombineOption(const String & value)
{
	if (value == PROPERTY_KEY_COLCOMBINEAVERAGE)
	{
		return PhysicMaterialCombine::Average;
	}
	else if (value == PROPERTY_KEY_COLCOMBINEMIN)
	{
		return PhysicMaterialCombine::Minimum;
	}
	else if (value == PROPERTY_KEY_COLCOMBINEMAX)
	{
		return PhysicMaterialCombine::Maximum;
	}
	else if (value == PROPERTY_KEY_COLCOMBINEMULTIPLY)
	{
		return PhysicMaterialCombine::Multiply;
	}

	ASSERT(0);
	return PhysicMaterialCombine::Average;
}