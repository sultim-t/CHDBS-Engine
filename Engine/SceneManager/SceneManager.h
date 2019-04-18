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
	// Returns its ID
	int CreateEmptyScene(const char *path);

	// Create scene from file
	// Returns its ID
	int CreateScene(const char *path);

	// Load scene by ID
	void LoadScene(int id);
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