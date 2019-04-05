#include "Scene.h"

Scene::Scene(const String &name, int id) 
	: name(name), sceneId(id) { }

void Scene::Init()
{
	entities.Init(128);
	rigidbodies.Init(128);
	colliders.Init(128);
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

int Scene::GetID() const
{
	return sceneId;
}

const String &Scene::GetName() const
{
	return name;
}