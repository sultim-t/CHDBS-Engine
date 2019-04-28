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
#include <Engine/Components/CBoxCollider.h>
#include <Engine/Components/CSphereCollider.h>
#include <Engine/Components/CMeshCollider.h>

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
	//updates.Init();
	fixedUpdates.Init();

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

	//updates.Delete();
	fixedUpdates.Delete();

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

	// also, call OnSceneLoading() on each component on each entity
	int entityCount = entities.GetSize();
	for (int i = 0; i < entityCount; i++)
	{
		const DynamicArray<IComponent*> &components = entities[i]->GetAllComponents();
		int componentCount = components.GetSize();

		for (int j = 0; j < componentCount; j++)
		{
			components[j]->OnSceneLoading(this->sceneId);
		}
	}

	// register components for update
	//ComponentSystem::Instance().RegisterUpdates(&updates);
	// register components for fixed update
	ComponentSystem::Instance().RegisterFixedUpdates(&fixedUpdates);
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
			bool toAdd = true;

			// check that only one rigidbody in the entity
			for (int j = 0; j < all.GetSize(); j++)
			{
				if (all[j]->IsClassType(Rigidbody::Type) && i != j)
				{
					String err = "Scene::More than one rigidbody : ";
					err += entity->GetName();
					Logger::Print(err);

					// ignore other rigidbodies
					toAdd = false;
				}
			}

			if (toAdd)
			{
				rigidbodies.Push((Rigidbody*)component);
			}
		}
		else if (component->IsClassType(CMeshCollider::Type))
		{
			bool toAdd = true;

			for (int j = 0; j < all.GetSize(); j++)
			{
				if (all[j]->IsClassType(Rigidbody::Type))
				{
					String err = "Scene::Mesh collider can't be with rigidbody : ";
					err += entity->GetName();
					Logger::Print(err);

					toAdd = false;
				}
			}

			// if there is no rigidbodies, add to colliders list
			if (toAdd)
			{
				colliders.Push((CMeshCollider*)component);
			}
		}
		else if (component->IsClassType(CBoxCollider::Type))
		{
			bool toAdd = true;

			for (int j = 0; j < all.GetSize(); j++)
			{
				if (all[j]->IsClassType(Rigidbody::Type))
				{
					// will be added to rigidbodies list
					toAdd = false;
				}
			}

			// if there is no rigidbodies, add to colliders list
			if (toAdd)
			{
				colliders.Push((CBoxCollider*)component);
			}
		}
		else if (component->IsClassType(CSphereCollider::Type))
		{
			bool toAdd = true;

			for (int j = 0; j < all.GetSize(); j++)
			{
				if (all[j]->IsClassType(Rigidbody::Type))
				{
					// will be added to rigidbodies list
					toAdd = false;
				}
			}

			// if there is no rigidbodies, add to colliders list
			if (toAdd)
			{
				colliders.Push((CSphereCollider*)component);
			}
		}
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

//Event &Scene::GetUpdateEvent()
//{
//	return updates;
//}

Event &Scene::GetFixedUpdateEvent()
{
	return fixedUpdates;
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
