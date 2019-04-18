#include "ComponentSystem.h"

ComponentSystem::ComponentSystem()
{ }

ComponentSystem::~ComponentSystem()
{ }

void ComponentSystem::Init()
{
}

void ComponentSystem::Update()
{
	if (entities == nullptr)
	{
		return;
	}

	for (int i = 0; i < entities->GetSize(); i++)
	{
		Entity *entity = entities->operator[](i);

		if (!entity->IsActive())
		{
			continue;
		}
	
		auto &components = entity->GetAllComponents();

		for (int i = 0; i < components.GetSize(); i++)
		{
			IComponent *c = components[i];

			if (c->IsActive())
			{
				c->Update();
			}
		}
	}
}

void ComponentSystem::FixedUpdate()
{
}

void ComponentSystem::Register(const DynamicArray<Entity*> *entities)
{
	this->entities = entities;
}

void ComponentSystem::Reset()
{
	entities = nullptr;
}

ComponentSystem &ComponentSystem::Instance()
{
	static ComponentSystem instance;
	return instance;
}
