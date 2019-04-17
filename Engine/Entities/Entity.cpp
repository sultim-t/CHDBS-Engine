#include "Entity.h"
#include <Engine/Systems/ComponentSystem.h>
#include <Engine/Memory/Memory.h>
#include <Engine/DataStructures/DynamicArray.h>

void Entity::AddComponent(IComponent *c)
{
	components.Push(c);
}

Entity::Entity(EntityID id) :
	entityId(id), isActive(true) // by default entities are active
{ }

Entity::~Entity()
{
	Destroy();
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
{
	// its components were allocated in EntityFactory,
	// their destruction is here
	
	for (int i = 0; i < components.GetSize(); i++)
	{
		delete components[i];
	}
}

void Entity::SetActive(bool active)
{
	isActive = true;
}

bool Entity::IsActive() const
{
	return isActive;
}

EntityID Entity::GetID() const
{
	return entityId;
}

const String &Entity::GetName() const
{
	return name;
}

Transform &Entity::GetTransform()
{
	return transform;
}

const Transform &Entity::GetTransform() const
{
	return transform;
}
