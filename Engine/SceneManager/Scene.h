#pragma once
#include <Engine/DataStructures/DynamicArray.h>

class Scene
{
	friend class SceneManager;

private:
	// Unique index of scene
	int		sceneId;
	String	name;

	// Stores all entities
	DynamicArray<Entity*>		entities;
	// Stores all rigidbodies with colliders
	DynamicArray<Rigidbody*>	rigidbodies;
	// Stores all colliders without rigidbodies
	DynamicArray<ICollider*>	colliders;

public:
	bool IsLoaded;

private:
	// Constructor
	Scene(const String &name, int id);
	// Initialize data structures
	void Init();

public:
	// Get all entities in current scene
	const DynamicArray<Entity*> &GetAllEntities() const;
	// Get all rigidbodies
	const DynamicArray<Rigidbody*> &GetAllRigidbodies() const;
	// Get all colliders without rigidbodies
	const DynamicArray<ICollider*> &GetAllColliders() const;

	// Get scene's ID
	int GetID() const;
	// Get scene's name
	const String &GetName() const;
};