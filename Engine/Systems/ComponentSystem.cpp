#include "ComponentSystem.h"

ComponentSystem::ComponentSystem()
{ }

ComponentSystem::~ComponentSystem()
{ 
	FOREACHLINKEDLIST(Entity*, entityPtr, entities)
	{
		Entity *entity = *entityPtr;

		for (auto c : entity->GetAllComponents())
		{
			delete c;
		}

		delete (*entityPtr);
	}
}

void ComponentSystem::Init()
{ }

void ComponentSystem::Update()
{
	FOREACHLINKEDLIST(Entity*, entityPtr, entities)
	{
		Entity *entity = *entityPtr;

		if (!entity->IsActive())
		{
			continue;
		}
	
		using namespace std;
		vector<IComponent*> components = entity->GetAllComponents();

		for (auto c : components)
		{
			if (c->IsActive())
			{
				c->Update();
			}
		}
	}
}

void ComponentSystem::Register(Entity * entity)
{
	entities.Add(entity);
}

ComponentSystem & ComponentSystem::Instance()
{
	static ComponentSystem instance;
	return instance;
}
