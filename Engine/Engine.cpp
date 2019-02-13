#include "Engine.h"
#include <Engine/Systems/ISystem.h>
#include <Engine/Rendering/ContextWindow.h>
#include <Engine/Systems/RenderingSystem.h>

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
}

void Engine::MainLoop()
{
	while (isActive)
	{
		Time::Calculate();
		Update();

		if (!ContextWindow::Instance().ShouldClose())
		{
			isActive = false;
		}
	}
}

void Engine::Update()
{
	RenderingSystem::Instance().Update();
}