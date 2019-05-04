#include "SceneManager.h"
#include <Engine/ResourceManager/ResourceManager.h>

SceneManager::SceneManager() : currentSceneIndex(-1)
{ }

SceneManager::~SceneManager()
{
	// delete each allocated scene
	for (int i = 0; i < scenes.GetSize(); i++)
	{
		delete scenes[i];
	}
}

void SceneManager::Init()
{
	scenes.Init(32);
}

int SceneManager::CreateEmptyScene(const char *name)
{
	// allocate
	int newId = scenes.GetSize();
	Scene *scene = new Scene(name, newId);
	scene->Init();
	
	// add to list
	scenes.Push(scene);

	return scene->GetID();
}

int SceneManager::CreateScene(const char *path)
{
	// load scene resource
	const SceneResource *sceneResource = ResourceManager::Instance().LoadScene(path);
	
	// allcate scene
	int sceneId = CreateEmptyScene(sceneResource->GetName());
	Scene *currentScene = scenes[sceneId];

	const StaticArray<SceneEntity> &entitiesData = sceneResource->GetEntitiesData();
	UINT count = entitiesData.GetSize();

	// for each entity
	for (UINT i = 0; i < count; i++)
	{
		// load it to scene,
		// there will be created an entity from this path

		// if transform is specified, apply it to current entity
		currentScene->CreateEntity(
			entitiesData[i].EntityPath,
			entitiesData[i].IsTransformed ? &entitiesData[i].Transformation : nullptr);
	}

	return sceneId;
}

void SceneManager::LoadScene(int index)
{
	ASSERT(index >= 0 && index < scenes.GetSize());

	// unload current if exist
	if (currentSceneIndex > 0)
	{
		scenes[currentSceneIndex]->Unload();
	}

	// load new
	scenes[index]->Load();
	currentSceneIndex = index;
}

void SceneManager::LoadScene(const Scene &scene)
{
	LoadScene(scene.GetID());
}

void SceneManager::LoadScene(const char *name)
{
	for (int i = 0; i < scenes.GetSize(); i++)
	{
		// if names are equal
		if (scenes[i]->GetName() == name)
		{
			LoadScene(scenes[i]->GetID());
			return;
		}
	}

	Logger::Print("No scene found");
}

Scene &SceneManager::GetCurrentScene() const
{
	return GetScene(currentSceneIndex);
}

Scene &SceneManager::GetScene(int id) const
{
	ASSERT(id >= 0 && id < scenes.GetSize());

	return *scenes[id];
}

SceneManager &SceneManager::Instance()
{
	static SceneManager instance;
	return instance;
}
