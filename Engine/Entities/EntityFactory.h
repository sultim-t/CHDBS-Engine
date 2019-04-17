#pragma once

#include <Engine/Entities/Entity.h>
#include <Engine/DataStructures/DynamicArray.h>
#include <Engine/DataStructures/HashTable.h>

class EntityFactory
{
	friend class Engine;

private:
	EntityID lastId;

	// Stores pointers to component creators
	HashTable<String, IComponentCreator> compCreators;

	// TODO : delete, must be in scene
	// Stores all entities
	DynamicArray<Entity*> entities;

	// TODO: this hash table stores 
	// "blueprints" for entities, but not the instances
	HashTable<String, Entity*> prefabs;

private:
	EntityID GetNextEntityID();
	
	EntityFactory();
	~EntityFactory();

	EntityFactory(EntityFactory&) = delete;
	EntityFactory(EntityFactory&&) = delete;
	EntityFactory &operator=(const EntityFactory&) = delete;

	// Creates component from xml node
	IComponent *CreateComponent(void *xmlElem);

	// Sets data to entity from xml node
	void SetData(Entity *entity, void *xmlElem);
	// Sets data to entity from xml node
	void SetData(Entity *entity, const Entity *source);

	// Parses XML file and creates entity
	Entity *PCreateEntity(const char *resource);
	// Creates copy of the entity
	Entity *PCreateEntity(const Entity *entity);

	template<class T>
	inline void PRegisterComponentType(const String &name);
	
	// Get instance
	static EntityFactory &Instance();

public:
	// Parses XML file and creates entity
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
	Instance().PRegisterComponentType<T>();
}
