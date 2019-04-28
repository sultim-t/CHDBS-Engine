#pragma once
#include "ISystem.h"

#include <Engine/DataStructures/DynamicArray.h>
#include <Engine/Entities/Entity.h>
#include <Engine/Components/IComponent.h>
#include <Engine/Base/Event.h>

// Manages all entities and updates their components
class ComponentSystem : public ISystem
{
private:
	// Pointer to entities storage
	const DynamicArray<Entity*> *entities;

	// Pointer to event, which is calling Update function in components
	//const Event					*updates;

	// Pointer to event, which is calling Fixed Update function in components
	const Event					*fixedUpdates;

private:
	ComponentSystem();
	~ComponentSystem();

	ComponentSystem(ComponentSystem&) = delete;
	ComponentSystem(ComponentSystem&&) = delete;
	ComponentSystem &operator=(const ComponentSystem&) = delete;

public:
	// Init structures
	void Init() override;

	// Update updatable objects' active components 
	void Update() override;

	// Update fixed updatable components
	void FixedUpdate();

	// Set pointer to entities array to update them
	void Register(const DynamicArray<Entity*> *entities);

	//// Set pointer to update event
	//void RegisterUpdates(const Event *updateEvent);
	// Set pointer to fixed update event
	void RegisterFixedUpdates(const Event *fixedUpdateEvent);

	// Set pointers to null
	void Reset();

	// Get instance of system
	static ComponentSystem &Instance();
};