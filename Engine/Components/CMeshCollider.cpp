#include "CMeshCollider.h"
#include "CCollider.h"

CLASSDEFINITION(IComponent, CMeshCollider)

void CMeshCollider::Init()
{ }

void CMeshCollider::Update()
{ }

void CMeshCollider::SetProperty(const String & key, const String & value)
{
	if (CCollider::SetProperty(this, key, value))
	{
		// if was set in CCollider
		return;
	}
}
