#pragma once
#include "ISystem.h"

#include <Engine/DataStructures/DynamicArray.h>
#include <Engine/Entities/Entity.h>
#include <Engine/Components/IComponent.h>

// Manages all entities and updates their components
class ComponentSystem : public ISystem
{
private:
	// Pointer to entities storage
	const DynamicArray<Entity*> *entities;

private:
	ComponentSystem();
	~ComponentSystem();

	ComponentSystem(ComponentSystem&) = delete;
	ComponentSystem(ComponentSystem&&) = delete;
	ComponentSystem &operator=(const ComponentSystem&) = delete;

public:
	// Init structures
	void Init() override;
	// Update each active component
	void Update() override;

	void Register(const DynamicArray<Entity*> *entities);

	// Get instance of system
	static ComponentSystem &Instance();
};