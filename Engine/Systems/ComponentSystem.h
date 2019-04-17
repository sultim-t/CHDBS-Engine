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

	// Update IUpdatable objects' active components 
	void Update() override;
	// Update IFixedUpdatable objects' active components 
	void FixedUpdate();

	// Set pointer to entities array to update them
	void Register(const DynamicArray<Entity*> *entities);
	// Set pointer to entities to null
	void Reset();

	// Get instance of system
	static ComponentSystem &Instance();
};