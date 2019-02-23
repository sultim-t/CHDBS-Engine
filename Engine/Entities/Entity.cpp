#include "Entity.h"
#include <Engine/Systems/ComponentSystem.h>
#include <Engine/Memory/Memory.h>
#include <Engine/DataStructures/DynamicArray.h>

void Entity::AddComponent(IComponent *c)
{
	components.Push(c);
}

Entity::Entity(EntityID id)
{
	entityId = id;
	isActive = true; // by default entities are active
}

void Entity::Init()
{ 
	// init container for components
	components.Init(8);
}

const DynamicArray<IComponent*> &Entity::GetAllComponents() const
{
	return components;
}

void Entity::Destroy()
{ }

void Entity::SetActive(bool active)
{
	isActive = true;
}

bool Entity::IsActive()
{
	return isActive;
}

EntityID Entity::GetID() const
{
	return entityId;
}

Transform &Entity::GetTransform()
{
	return transform;
}

const Transform &Entity::GetTransform() const
{
	return transform;
}
