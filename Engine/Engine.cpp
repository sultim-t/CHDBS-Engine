#include "Engine.h"
#include <Engine/Systems/ISystem.h>
#include <Engine/Rendering/ContextWindow.h>
#include <Engine/Systems/RenderingSystem.h>
#include <Engine/Systems/ComponentSystem.h>
#include <Engine/Systems/PhysicsSystem.h>
#include <Engine/Systems/InputSystem.h>
#include <Engine/ResourceManager/ResourceManager.h>

Engine::Engine()
{
	isActive = true;
}

Engine::~Engine()
{
	isActive = false;
}

void Engine::Init()
{
	// create contrext window
	ContextWindow::Instance().Init("Engine", 1280, 720);

	// init all systems
	ResourceManager::Instance().Init();
	RenderingSystem::Instance().Init();
	ComponentSystem::Instance().Init();
	PhysicsSystem::Instance().Init();
	InputSystem::Instance().Init();
}

void Engine::MainLoop()
{	
	// Recalculate time, there shouldn't be counted initialization time
	Time::Init();

	while (isActive)
	{
		// recalculate time
		Time::Calculate();
	
		// updated in fixed update
		while (Time::ToFixedUpdate())
		{
			FixedUpdate();
		}

		// updated each frame
		Update();

		if (!ContextWindow::Instance().ShouldClose())
		{
			isActive = false;
		}
	}

	// destroy
	ContextWindow::Instance().Terminate();
}

void Engine::Update()
{
	// update inputs
	InputSystem::Instance().Update();

	// render all objects in current scene
	RenderingSystem::Instance().Update();

	// update components in current scene
	ComponentSystem::Instance().Update();
}

void Engine::FixedUpdate()
{
	// update all physics objects
	// check their collisions, solve them
	PhysicsSystem::Instance().Update();
}