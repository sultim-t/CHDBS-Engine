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
	//if (updates == nullptr)
	//{
	//	return;
	//}

	//const DynamicArray<IDelegate*> &subscribers = updates->GetSubscribers();
	//int count = subscribers.GetSize();

	//for (int i = 0; i < count; i++)
	//{
	//	subscribers[i]->Invoke();
	//}


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
	if (fixedUpdates != nullptr)
	{
		fixedUpdates->operator()();
	}
}

void ComponentSystem::Register(const DynamicArray<Entity*> *entities)
{
	this->entities = entities;
}

//void ComponentSystem::RegisterUpdates(const Event * updateEvent)
//{
//	this->updates = updateEvent;
//}

void ComponentSystem::RegisterFixedUpdates(const Event * fixedUpdateEvent)
{
	this->fixedUpdates = fixedUpdateEvent;
}

void ComponentSystem::Reset()
{
	this->entities = nullptr;
	this->fixedUpdates = nullptr;
}

ComponentSystem &ComponentSystem::Instance()
{
	static ComponentSystem instance;
	return instance;
}
