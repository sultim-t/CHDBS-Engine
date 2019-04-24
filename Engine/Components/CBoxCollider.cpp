#include "CBoxCollider.h"
#include "CCollider.h"

CLASSDEFINITION(IComponent, CBoxCollider)

void CBoxCollider::Init()
{ 
	// set current transform in collider
	// this->SetTransform(&GetOwner().GetTransform());
}

void CBoxCollider::Update()
{ }


// for aabbs
#define PROPERTY_KEY_COLMAX		"AABBMax"
#define PROPERTY_KEY_COLMIN		"AABBMin"
#define PROPERTY_KEY_COLOFFSET	"Offset"

void CBoxCollider::SetProperty(const String & key, const String & value)
{
	if (CCollider::SetProperty(this, key, value))
	{
		// if was set in CCollider
		return;
	}
	
	if (key == PROPERTY_KEY_COLOFFSET)
	{
		this->GetAABBRef().Move(value.ToVector3());
	}
	else if (key == PROPERTY_KEY_COLMAX)
	{
		this->GetAABBRef().SetMax(value.ToVector3());
	}
	else if (key == PROPERTY_KEY_COLMIN)
	{
		this->GetAABBRef().SetMin(value.ToVector3());
	}
}

const Transform &CBoxCollider::GetTransform() const
{
	return GetOwner().GetTransform();
}
