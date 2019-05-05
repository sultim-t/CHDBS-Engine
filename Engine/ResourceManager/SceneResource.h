#pragma once
#include <Engine/DataStructures/StaticArray.h>
#include <Engine/Math/Transform.h>

struct SceneEntity
{
	Transform	Transformation;
	String		EntityPath;		// path to entity resource
	bool		IsTransformed;	// does this entity have overrided transform
};

class SceneResource
{
	friend class ResourceManager;

private:
	// Scene's entities data
	StaticArray<SceneEntity>	entitiesData;
	// Scene's skybox names
	StaticArray<String>			skyboxSideNames;

	// Scene's name
	String sceneName;
	// Path to this resource
	String path;

public:
	// Default constructor
	SceneResource(const char *path);
	// Destructor
	~SceneResource();

	void Init(int entitiesCount, const char *name = "Scene");

	// Get all paths to entity files in this scene
	const StaticArray<SceneEntity> &GetEntitiesData() const;
	// Get all scene's skybox names
	const StaticArray<String> &GetSkyboxSideNames() const;
	// Get scene's name
	const String &GetName() const;
	// Get path to this scene resource
	const String &GetPath() const;
};

inline SceneResource::SceneResource(const char * path) 
	: path(path) { }

inline SceneResource::~SceneResource()
{
	// delete strings manually 
	// (because they will not be destroyed during array destruction)
	// (calling Delete on empty string does nothing)
	for (UINT i = 0; i < entitiesData.GetSize(); i++)
	{
		entitiesData[i].EntityPath.Delete();
	}

	for (int i = 0; i < 6; i++)
	{
		skyboxSideNames[i].Delete();
	}
}

inline void SceneResource::Init(int entitiesCount, const char *name)
{
	sceneName = name;
	entitiesData.Init(entitiesCount);

	for (int i = 0; i < entitiesCount; i++)
	{
		// set default
		entitiesData[i].EntityPath.Init("");
		entitiesData[i].IsTransformed = false;
		entitiesData[i].Transformation = Transform();
	}

	skyboxSideNames.Init(6);
	for (int i = 0; i < 6; i++)
	{
		skyboxSideNames[i].Init("");
	}
}

inline const StaticArray<SceneEntity> &SceneResource::GetEntitiesData() const
{
	return entitiesData;
}

inline const StaticArray<String> &SceneResource::GetSkyboxSideNames() const
{
	return skyboxSideNames;
}

inline const String &SceneResource::GetName() const
{
	return sceneName;
}

inline const String &SceneResource::GetPath() const
{
	return path;
}
