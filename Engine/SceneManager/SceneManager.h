#pragma once
#include "Scene.h"

#include <Engine/DataStructures/DynamicArray.h>

class SceneManager
{
private:
	// All loaded scenes
	// Scenes' indices in this array are their IDs
	DynamicArray<Scene*> scenes;

	// Current scene's index in array
	int currentSceneIndex;

public:
	// Destroys all scenes
	~SceneManager();

	// Allocate memory for the fields
	void Init();

	// Create empty scene
	Scene &CreateScene(const String &name);

	// Load scene by index
	void LoadScene(int index);
	// Load scene
	void LoadScene(const Scene &scene);
	// Load scene by name
	void LoadScene(const char *name);

	// Get current active scene
	Scene &GetCurrentScene() const;
	// Get scene by index
	Scene &GetScene(int id) const;

	static SceneManager &Instance();
};