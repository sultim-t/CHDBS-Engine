#include "CSphereCollider.h"
#include "CCollider.h"

CLASSDEFINITION(IComponent, CSphereCollider)

void CSphereCollider::Init()
{ 
	// set current transform in collider
	this->SetTransform(&GetOwner().GetTransform());
}

void CSphereCollider::Update()
{ }


#define PROPERTY_KEY_COLRADIUS	"SphereRadius"
#define PROPERTY_KEY_COLOFFSET	"Offset"

void CSphereCollider::SetProperty(const String & key, const String & value)
{
	if (CCollider::SetProperty(this, key, value))
	{
		// if was set in CCollider
		return;
	}

	if (key == PROPERTY_KEY_COLOFFSET)
	{
		// change sphere's center
		this->GetSphereRef().SetCenter(value.ToVector3());
	}
	else if (key == PROPERTY_KEY_COLRADIUS)
	{
		// change sphere's center
		this->GetSphereRef().SetRadius(value.ToFloat());
	}
}