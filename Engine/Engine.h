#pragma once

#include <Engine/DataStructures/LinkedList.h>

class Engine
{
private:
	bool isActive;
	LinkedList<ISystem*> systems;

public:
	Engine();
	~Engine();

	void Init();
	// Calls Update function
	void MainLoop();
	// To update all systems
	void Update();
	void AddSystem(ISystem *system);
};