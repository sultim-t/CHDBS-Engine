#include "Engine.h"
#include <Engine/Systems/ISystem.h>
#include <Engine/Rendering/ContextWindow.h>
#include <Engine/Systems/RenderingSystem.h>
#include <Engine/Systems/ComponentSystem.h>
#include <Engine/Systems/PhysicsSystem.h>

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
	ContextWindow::Instance().Init("Engine", 1280, 720);
	RenderingSystem::Instance().Init();
	ComponentSystem::Instance().Init();
	PhysicsSystem::Instance().Init();
}

void Engine::MainLoop()
{
	while (isActive)
	{
		Time::Calculate();
		while (Time::ToFixedUpdate())
		{
			FixedUpdate();
		}

		Update();

		if (!ContextWindow::Instance().ShouldClose())
		{
			isActive = false;
		}
	}

	ContextWindow::Instance().Terminate();
}

void Engine::Update()
{
	RenderingSystem::Instance().Update();
	ComponentSystem::Instance().Update();
}

void Engine::FixedUpdate()
{
	PhysicsSystem::Instance().Update();
}