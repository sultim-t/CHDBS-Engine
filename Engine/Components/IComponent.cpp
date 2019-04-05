#include "IComponent.h"

// Init value
const UINT IComponent::Type = HASHSTRING(TOSTRING(IComponent));

bool IComponent::IsClassType(const UINT classType) const
{
	return classType == Type;
}

void IComponent::SetOwner(Entity * owner)
{
	this->owner = owner;
}

void IComponent::Enable()
{
	isActive = true;
}

void IComponent::Disable()
{
	isActive = false;
}

bool IComponent::IsActive() const
{
	return isActive && owner->IsActive();
}

const Entity &IComponent::GetOwner() const
{
	return *owner;
}

Entity &IComponent::GetOwner()
{
	return *owner;
}