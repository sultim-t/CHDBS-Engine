#pragma once

#include <Engine/Entities/Entity.h>
#include <Engine/DataStructures/HashTable.h>
#include <vector>
#include <map>

class EntityFactory
{
	friend class Engine;

private:
	EntityID lastId;
	HashTable<String, IComponentCreator> compCreators;

private:
	EntityID GetNextEntityID();
	EntityFactory();
	EntityFactory(EntityFactory&) = delete;
	EntityFactory(EntityFactory&&) = delete;

	IComponent *CreateComponent(void *xmlElem);

public:
	// Parses XML file and creates entity
	Entity *CreateEntity(const char *resource);
	// Get instance
	static EntityFactory &Instance();
};