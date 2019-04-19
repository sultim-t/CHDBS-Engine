#include "Scene.h"

#include <Engine/Entities/EntityFactory.h>
#include <Engine/Physics/Rigidbody.h>
#include <Engine/Components/CCamera.h>
#include <Engine/Components/CLight.h>
#include <Engine/Components/CModel.h>
#include <Engine/Components/CParticleSystem.h>
#include <Engine/Systems/ComponentSystem.h>
#include <Engine/Systems/PhysicsSystem.h>
#include <Engine/Systems/RenderingSystem.h>

Scene::Scene(const String &name, int id) 
	: name(name), sceneId(id) { }

Scene::~Scene()
{
	// destroy all data
	Destroy();
}

void Scene::Init()
{
	// allocate memory
	entities.Init(128);
	
	rigidbodies.Init(128);
	colliders.Init(128);

	cameras.Init(8);
	lights.Init(8);
	models.Init(128);
	particleSystems.Init(128);
}

void Scene::Destroy()
{
	// delete entities themselves
	for (int i = 0; i < entities.GetSize(); i++)
	{
		// delete entities
		delete entities[i];
	}

	// delete array
	entities.Delete();

	// delete arrays, but not the data,
	// becuase it is contained in entities
	rigidbodies.Delete();
	colliders.Delete();

	cameras.Delete();
	lights.Delete();
	models.Delete();
	particleSystems.Delete();
}

void Scene::Update()
{
	// update each component
	ComponentSystem::Instance().Update();
}

void Scene::FixedUpdate()
{
	// todo
}

void Scene::Load()
{
	// register current entities array in component system
	// to update them every frame
	ComponentSystem::Instance().Register(&entities);

	// register rigidbodies and colliders
	PhysicsSystem::Instance().Register(&rigidbodies, &colliders);

	// register objects for rendering
	RenderingSystem::Instance().Register(&cameras);
	RenderingSystem::Instance().Register(&lights);
	RenderingSystem::Instance().Register(&models);
	RenderingSystem::Instance().Register(&particleSystems);
}

void Scene::Unload()
{	
	// remove pointer to entities array from component system
	// it prevents updating
	ComponentSystem::Instance().Reset();

	// remove pointer from physics system
	PhysicsSystem::Instance().Reset();

	// remove pointer from rendering system
	RenderingSystem::Instance().Reset();
}

void Scene::AddEntity(Entity * entity)
{
	// add entity to list
	entities.Push(entity);

	// get all components in this entity
	const DynamicArray<IComponent*> &all = entity->GetAllComponents();

	// check for all needed components for physics and rendering
	for (int i = 0; i < all.GetSize(); i++)
	{
		IComponent *component = all[i];

		if (component->IsClassType(Rigidbody::Type))
		{
			rigidbodies.Push((Rigidbody*)component);
		}
		// now all colliders are stored in rigidbodies
		// TODO: there MUST be collider as component to allow using of static colliders
		//else if (component->Type == Coll::Type)
		//{
		//	colliders.Push((ICollider*)component);
		//}
		else if (component->IsClassType(CModel::Type))
		{
			models.Push((CModel*)component);
		}
		else if (component->IsClassType(CParticleSystem::Type))
		{
			particleSystems.Push((CParticleSystem*)component);
		}
		else if (component->IsClassType(CCamera::Type))
		{
			cameras.Push((CCamera*)component);
		}
		else if (component->IsClassType(CLight::Type))
		{
			lights.Push((CLight*)component);
		}
	}
}

const DynamicArray<Entity*> &Scene::GetAllEntities() const
{
	return entities;
}

const DynamicArray<Rigidbody*> &Scene::GetAllRigidbodies() const
{
	return rigidbodies;
}

const DynamicArray<ICollider*> &Scene::GetAllColliders() const
{
	return colliders;
}

void Scene::CreateEntity(const char *name)
{
	// create entity in factory
	Entity *created = EntityFactory::CreateEntity(name);
	// add to this scene
	AddEntity(created);
}

void Scene::CreateEntity(const Entity *source)
{
	// create entity in factory
	Entity *created = EntityFactory::CreateEntity(source);
	// add to this scene
	AddEntity(created);
}

Entity *Scene::FindEntity(const char *name)
{
	int count = entities.GetSize();
	for (int i = 0; i < count; i++)
	{
		// check names
		if (entities[i]->GetName() == name)
		{
			return entities[i];
		}
	}

	return nullptr;
}

int Scene::GetID() const
{
	return sceneId;
}

const String &Scene::GetName() const
{
	return name;
}

const Skybox &Scene::GetSkybox() const
{
	return Skybox::Instance();
}
