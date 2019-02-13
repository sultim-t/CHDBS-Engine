#include "IComponent.h"

// Init value
const ComponentID IComponent::Type = HASHSTRING(TOSTRING(IComponent));

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

const Entity & IComponent::GetOwner() const
{
	return *owner;
}
