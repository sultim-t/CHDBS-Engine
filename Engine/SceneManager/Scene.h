#pragma once
#include <Engine/DataStructures/DynamicArray.h>

class Scene
{
	friend class SceneManager;

private:
	// Unique index of scene
	int sceneId;
	String name;
	// Stores all entities
	DynamicArray<Entity*> entities;

public:
	bool IsLoaded;

private:
	inline Scene(const String &name, int id);
	inline void Init();

public:
	// Get array with all entities in current scene
	inline const DynamicArray<Entity*> &GetAllEntities() const;
	// Get scene's ID
	inline int GetID() const;
	// Get scene's name
	inline const String &GetName() const;
};

inline Scene::Scene(const String &name, int id)
{ 
	this->name = name;
	this->sceneId = id;
}

inline void Scene::Init()
{
	entities.Init(64);
}

inline const DynamicArray<Entity*> &Scene::GetAllEntities() const
{
	return entities;
}

inline int Scene::GetID() const
{
	return sceneId;
}

inline const String &Scene::GetName() const
{
	return name;
}