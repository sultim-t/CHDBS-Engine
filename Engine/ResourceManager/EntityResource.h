#pragma once
#include "ComponentResource.h"

#include <Engine/DataStructures/StaticArray.h>
#include <Engine/Math/Transform.h>

#define ENTITY_KEY_ACTIVE "Active"

// Contains component resources and main enitity's data
class EntityResource
{
	friend class ResourceManager;

private:
	StaticArray<ComponentResource*> components;

	Transform	transform;
	String		path;
	String		name;
	bool		isActive;

public:	
	EntityResource();
	~EntityResource();

	void Init(int componentsCount);

	// Get all component resources in this entity
	const StaticArray<ComponentResource*> &GetComponents() const;

	// Get path to this entity resource
	const String	&GetPath() const;
	// Get name of this entity
	const String	&GetName() const;
	// Is this entity active
	bool			IsActive() const;
	// Get tranform of this entity
	const Transform	&GetTransform() const;
};