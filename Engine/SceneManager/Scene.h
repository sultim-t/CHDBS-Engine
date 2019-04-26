#pragma once
#include <Engine/DataStructures/DynamicArray.h>
#include <Engine/Rendering/Skybox.h>
#include <Engine/Base/Event.h>

// Delegate for fixed update call
typedef void(*FixedUpdateFunction)(int);

class Scene
{
	friend class ResourceManager;
	friend class SceneManager;

private:
	// Unique index of scene
	int		sceneId;
	String	name;

	// Event to subscribe, will be called on fixed update
	Event	fixedUpdates;

	// Stores all entities in this scene
	DynamicArray<Entity*>			entities;

	// References to all rigidbodies with colliders and colliders without rigidbodies
	// Rigidbodies and colliders themselves are stored in entities
	DynamicArray<Rigidbody*>		rigidbodies;
	DynamicArray<ICollider*>		colliders;

	// References to all objects related to rendering
	DynamicArray<CModel*>			models;
	DynamicArray<CCamera*>			cameras;
	DynamicArray<CParticleSystem*>	particleSystems;
	DynamicArray<CLight*>			lights;

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
	// Update all components which are using fixed step
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
	void CreateEntity(const char *name);
	// Create copy of entity in this scene
	void CreateEntity(const Entity *source);

	// Find entity in this scene by its name
	Entity *FindEntity(const char *name);

	// Subscribe component for fixed update
	void SubscribeFixedUpdate(const FixedUpdateFunction &func);

	// Get scene's ID
	int GetID() const;
	// Get scene's name
	const String &GetName() const;
	// Get scene's skybox
	const Skybox &GetSkybox() const;
};