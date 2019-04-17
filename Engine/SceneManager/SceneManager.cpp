#include "SceneManager.h"

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

Scene &SceneManager::CreateScene(const String &name)
{
	// allocate
	Scene *scene = new Scene(name, scenes.GetSize());
	scene->Init();
	
	// add to list
	scenes.Push(scene);

	return *scene;
}

void SceneManager::LoadScene(int index)
{
	ASSERT(index >= 0 && index < scenes.GetSize());

	// unload current
	scenes[currentSceneIndex]->Unload();

	// load new
	scenes[index]->Load();
	currentSceneIndex = index;
}

void SceneManager::LoadScene(const Scene &scene)
{
	LoadScene(scene.GetID());
}

void SceneManager::LoadScene(const char * name)
{
	for (int i = 0; i < scenes.GetSize(); i++)
	{
		// if names are equal
		if (scenes[i]->GetName() == name)
		{
			LoadScene(scenes[i]->GetID());
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
