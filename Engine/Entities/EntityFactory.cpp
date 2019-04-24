#include "EntityFactory.h"
#include <Engine/DataStructures/DynamicArray.h>
#include <Engine/Components/CCamera.h>
#include <Engine/Components/CFreeMovement.h>
#include <Engine/Components/CModel.h>
#include <Engine/Components/CSkinnedModel.h>
#include <Engine/Components/CLight.h>
#include <Engine/Components/CParticleSystem.h>
#include <Engine/Components/CBoxCollider.h>
#include <Engine/Components/CSphereCollider.h>
#include <Engine/Components/CMeshCollider.h>
#include <Engine/Physics/Rigidbody.h>
#include <Engine/Systems/ComponentSystem.h>
#include <Engine/ResourceManager/ResourceManager.h>

#include <Game/Scripts/Weapon.h>

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

EntityID EntityFactory::GetNextEntityID()
{
	lastId++;
	return lastId;
}

EntityFactory::EntityFactory()
{
	// init hash table
	prefabs.Init(64, 16);
	prefabs.DeclareHashFunction(String::StringHash);

	// Note: entities are stored in Scene class

	// register in component system
	// ComponentSystem::Instance().Register(&entities);
	
	// init hash table
	compCreators.Init(10, 6);
	compCreators.DeclareHashFunction(String::StringHash);

	// Builtin components
	PRegisterComponentType<CCamera>("CCamera");
	PRegisterComponentType<CFreeMovement>("CFreeMovement");
	PRegisterComponentType<CLight>("CLight");
	PRegisterComponentType<CModel>("CModel");
	PRegisterComponentType<CSkinnedModel>("CSkinnedModel");
	PRegisterComponentType<CParticleSystem>("CParticleSystem");

	PRegisterComponentType<Rigidbody>("Rigidbody");
	PRegisterComponentType<CMeshCollider>("CMeshCollider");
	PRegisterComponentType<CBoxCollider>("CBoxCollider");
	PRegisterComponentType<CSphereCollider>("CSphereCollider");
	
	PRegisterComponentType<CWeapon>("CWeapon");
}

EntityFactory::~EntityFactory()
{ }

IComponent *EntityFactory::CreateComponent(const ComponentResource &resource)
{
	IComponentCreator foundCreator;

	// try to find component creator
	if (compCreators.Find(resource.GetName(), foundCreator))
	{
		// if found, create component
		// and return it
		return foundCreator(resource);
	}
	else
	{
		// if can't find component creator
		return nullptr;
	}
}

Entity *EntityFactory::PCreateEntity(const char *resourcePath)
{
	Entity *entity;

	// try to find already loaded "blueprint"
	if (prefabs.Find(resourcePath, entity))
	{
		// TODO: entity copying

		// if exist
		// return CopyEntity(entity);
	}

	const EntityResource *entityResource = ResourceManager::Instance().LoadEnitity(resourcePath);

	// allocate
	entity = new Entity(GetNextEntityID());
	// allocate memory for fields
	entity->Init();

	// load main data (transformatrions, name etc)
	entity->name = entityResource->GetName();
	entity->isActive = entityResource->IsActive();
	entity->transform = entityResource->GetTransform();

	// init components
	const StaticArray<ComponentResource*> &componentResources = entityResource->GetComponents();
	for (UINT i = 0; i < componentResources.GetSize(); i++)
	{
		ComponentResource *componentResource = componentResources[i];

		// allocate, set all data from resource
		IComponent *comp = CreateComponent(*componentResource);
		
		// set active
		comp->isActive = componentResource->IsActive();

		// link
		entity->AddComponent(comp);
		comp->SetOwner(entity);

		// everything is set up
		comp->Init();
	}

	// register "blueprint"
	prefabs.Add(resourcePath, entity);

	return entity;
}

Entity * EntityFactory::PCreateEntity(const Entity *source)
{
	// allocate
	Entity *entity = new Entity(GetNextEntityID());
	// allocate memory for fields
	entity->Init();

	// load main data
	entity->name = source->name;
	entity->isActive = source->isActive;
	entity->transform = source->transform;

	auto &components = source->GetAllComponents();

	// foreach component in source
	for (int i = 0; i < components.GetSize(); i++)
	{
		// TODO: in component: abstract function Copy(IComponent &destination)
		// which copies data from current component
		ASSERT(0);
	}

	// Note : entity are not stored here anymore, see Scene class
	//// finally, store entity
	//entities.Push(entity);

	return entity;
}

EntityFactory &EntityFactory::Instance()
{
	static EntityFactory instance;
	return instance;
}

Entity *EntityFactory::CreateEntity(const char *resource)
{
	return EntityFactory::Instance().PCreateEntity(resource);
}

Entity *EntityFactory::CreateEntity(const Entity *entity)
{
	return EntityFactory::Instance().PCreateEntity(entity);
}
