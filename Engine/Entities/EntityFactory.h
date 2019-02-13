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
	// Stores pointers to component creators
	HashTable<String, IComponentCreator> compCreators;

private:
	EntityID GetNextEntityID();
	
	EntityFactory();

	EntityFactory(EntityFactory&) = delete;
	EntityFactory(EntityFactory&&) = delete;
	EntityFactory &operator=(const EntityFactory&) = delete;

	// Creates component from xml node
	IComponent *CreateComponent(void *xmlElem);
	// Parses XML file and creates entity
	Entity *PCreateEntity(const char *resource);
	// Get instance
	static EntityFactory &Instance();

public:
	// Parses XML file and creates entity
	static Entity *CreateEntity(const char *resource);
};