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

void SceneManager::LoadScene(const Scene &scene)
{
	// todo
	ASSERT(0);
}

void SceneManager::LoadScene(int index)
{
	ASSERT(index >= 0 && index < scenes.GetSize());
	LoadScene(*scenes[index]);
}

SceneManager &SceneManager::Instance()
{
	static SceneManager instance;
	return instance;
}
