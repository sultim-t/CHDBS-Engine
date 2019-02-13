#pragma once
#include "ISystem.h"

#include <Engine/DataStructures/LinkedList.h>
#include <Engine/Entities/Entity.h>
#include <Engine/Components/IComponent.h>

// Manages all entities and updates their components
class ComponentSystem : public ISystem
{
private:
	LinkedList<Entity*> entities;

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

	void Register(Entity *entity);

	// Get instance of system
	static ComponentSystem &Instance();
};