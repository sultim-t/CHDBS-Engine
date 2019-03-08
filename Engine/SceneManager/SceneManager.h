#pragma once
#include "Scene.h"

#include <Engine/DataStructures/DynamicArray.h>

class SceneManager
{
private:
	DynamicArray<Scene*> scenes;
	int currentSceneId;

public:
	~SceneManager();

	void Init();

	// Create empty scene
	Scene &CreateScene(const String &name);

	// Load scene
	void LoadScene(const Scene &scene);
	// Load scene by index
	void LoadScene(int index);

	// Get current active scene
	inline Scene &GetCurrentScene() const;
	// Get scene by index
	inline Scene &GetScene(int id) const;

	static SceneManager &Instance();
};

inline Scene &SceneManager::GetCurrentScene() const
{
	return GetScene(currentSceneId);
}

inline Scene &SceneManager::GetScene(int id) const
{
	ASSERT(id >= 0 && id < scenes.GetSize());
	
	return *scenes[id];
}