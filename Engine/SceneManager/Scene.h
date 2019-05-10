#pragma once
#include <Engine/DataStructures/DynamicArray.h>
#include <Engine/Rendering/Skybox.h>
#include <Engine/Base/Event.h>
#include <Engine/Entities/Entity.h>

#include <Engine/Rendering/ILight.h>
#include <Engine/Rendering/IModel.h>
#include <Engine/Rendering/ICamera.h>
#include <Engine/Rendering/IParticleSystem.h>

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


	// Event to subscribe, will be called on frame change
	//Event							updates;
	// Event to subscribe, will be called on fixed update
	Event							fixedUpdates;

	// Stores all entities in this scene
	DynamicArray<Entity*>			entities;

	// References to all rigidbodies with colliders and colliders without rigidbodies
	// Rigidbodies and colliders themselves are stored in entities
	DynamicArray<Rigidbody*>		rigidbodies;
	DynamicArray<ICollider*>		colliders;

	// References to all objects related to rendering
	DynamicArray<IModel*>			models;
	DynamicArray<ICamera*>			cameras;
	DynamicArray<IParticleSystem*>	particleSystems;
	DynamicArray<ILight*>			lights;

	Cubemap							*skybox;

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
	void CreateEntity(const char *name, const Transform *transform = nullptr);
	// Create copy of entity in this scene
	void CreateEntity(const Entity *source);

	// Find entity in this scene by its name
	Entity *FindEntity(const char *name);

	// Subscribe component for fixed update
	//Event &GetUpdateEvent();
	// Subscribe component for fixed update
	Event &GetFixedUpdateEvent();

	// Get scene's ID
	int GetID() const;
	// Get scene's name
	const String &GetName() const;
	// Get scene's skybox
	const Skybox &GetSkybox() const;
};