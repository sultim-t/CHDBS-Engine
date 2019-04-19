#include "EntityResource.h"

EntityResource::EntityResource()
{ }

EntityResource::~EntityResource()
{
	// delete each componenet resource
	for (UINT i = 0; i < components.GetSize(); i++)
	{
		delete components[i];
	}
}

void EntityResource::Init(int componentsCount)
{
	components.Init(componentsCount);
}

const StaticArray<ComponentResource*> &EntityResource::GetComponents() const
{
	return components;
}

const String &EntityResource::GetPath() const
{
	return path;
}

const String &EntityResource::GetName() const
{
	return name;
}

bool EntityResource::IsActive() const
{
	return isActive;
}

const Transform &EntityResource::GetTransform() const
{
	return transform;
}
