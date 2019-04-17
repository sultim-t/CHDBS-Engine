#pragma once
#include <Engine/DataStructures/StaticArray.h>

class SceneResource
{
	friend class ResourceManager;

private:
	// Paths to entities files
	StaticArray<String> entityPaths;

	// Scene's name
	String sceneName;
	// Path to this resource
	String path;

public:
	// Default constructor
	SceneResource(const char *path);
	// Destructor
	~SceneResource();

	// Get all paths to entity files in this scene
	const StaticArray<String> &GetEntityPaths() const;
};

inline SceneResource::SceneResource(const char * path) 
	: path(path) { }

inline SceneResource::~SceneResource()
{
	// delete strings manually 
	// (because they wil not be destroyed during array destruction)

	for (UINT i = 0; i < entityPaths.GetSize(); i++)
	{
		entityPaths[i].Delete();
	}
}

inline const StaticArray<String>& SceneResource::GetEntityPaths() const
{
	return entityPaths;
}