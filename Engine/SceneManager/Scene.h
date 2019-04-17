#pragma once
#include <Engine/DataStructures/DynamicArray.h>

class Scene
{
	friend class SceneManager;

private:
	// Unique index of scene
	int		sceneId;
	String	name;

	// Stores all entities in this scene
	DynamicArray<Entity*>		entities;

	// References to all rigidbodies with colliders
	// Rigidbodies themselves are stored in entities
	DynamicArray<Rigidbody*>	rigidbodies;
	// References to all colliders without rigidbodies
	// Colliders themselves are stored in entities
	DynamicArray<ICollider*>	colliders;

public:
	bool IsLoaded;

private:
	// Constructor
	Scene(const String &name, int id);
	// Destructor
	~Scene();
	
	// Initialize data structures
	void Init();
	// Destroy entities in this scene
	void Destroy();

	// Update all components per frame
	void Update();
	// Update all physics
	void FixedUpdate();

	// Load all resources needed for entities in this scene
	void Load();
	// Unloads scene, but does't destroy it
	void Unload();

	// Add entity to this scene by registering its components to update them.
	// Also, if needed, registers it in the physics system
	void AddEntity(Entity *entity);

public:
	// Get all entities in current scene
	const DynamicArray<Entity*> &GetAllEntities() const;
	// Get all rigidbodies
	const DynamicArray<Rigidbody*> &GetAllRigidbodies() const;
	// Get all colliders without rigidbodies
	const DynamicArray<ICollider*> &GetAllColliders() const;

	// Create entity from file in this scene
	void CreateEntity(const String &name);
	// Create copy of entity in this scene
	void CreateEntity(const Entity *source);

	// Find entity in this scene by its name
	Entity *FindEntity(const String &name);

	// Get scene's ID
	int GetID() const;
	// Get scene's name
	const String &GetName() const;
};