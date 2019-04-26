#pragma once

#include <Engine/Entities/Entity.h>
#include <Engine/DataStructures/DynamicArray.h>
#include <Engine/DataStructures/StaticArray.h>
#include <Engine/DataStructures/HashTable.h>
#include <Engine/Systems/ComponentSystem.h>

class EntityFactory
{
	friend class Engine;

private:
	EntityID lastId;

	// Stores pointers to component creators
	HashTable<String, IComponentCreator> compCreators;

	// This hash table stores 
	// "blueprints" for entities, but not the instances
	HashTable<String, Entity*> prefabs;

private:
	EntityID GetNextEntityID();
	
	EntityFactory();
	~EntityFactory();

	EntityFactory(EntityFactory&) = delete;
	EntityFactory(EntityFactory&&) = delete;
	EntityFactory &operator=(const EntityFactory&) = delete;

	// Creates component from resource
	IComponent *CreateComponent(const ComponentResource &resource);

	// Loades entity resource and creates entity
	Entity *PCreateEntity(const char *resource);
	
	// TODO: creating copy of entity
	// Creates copy of the entity
	Entity *PCreateEntity(const Entity *entity);

	template<class T>
	inline void PRegisterComponentType(const String &name);
	
	// Get instance
	static EntityFactory &Instance();

public:
	// Loades entity resource and creates entity
	static Entity *CreateEntity(const char *resource);

	// Creates copy of the entity
	static Entity *CreateEntity(const Entity *entity);

	// Call this function to register component type
	// It will automatically generate a component creator
	// which will call SetProperty function the component
	template<class T>
	static void RegisterComponentType(const String &name);
};

template<class T>
inline void EntityFactory::PRegisterComponentType(const String &name)
{
	compCreators.Add(name, CompCreator<T>);
}

template<class T>
inline void EntityFactory::RegisterComponentType(const String &name)
{
	Instance().PRegisterComponentType<T>(name);
}

template <class T>
IComponent *CompCreator(const ComponentResource &componentResource);

template<class T>
IComponent *CompCreator(const ComponentResource &componentResource)
{
	IComponent *c = new T();

	// get all tuples
	const StaticArray<StringTuple> &tuples = componentResource.GetKeysValues();

	// for each key-value
	for (UINT i = 0; i < tuples.GetSize(); i++)
	{
		// set property to component
		c->SetProperty(tuples[i].Left(), tuples[i].Right());
	}

	return c;
}
