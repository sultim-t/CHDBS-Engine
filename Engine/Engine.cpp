#include "Engine.h"
#include <Engine/Systems/ISystem.h>
#include <Engine/Rendering/ContextWindow.h>

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
	}
}

void Engine::Update()
{
	FOREACHLINKEDLIST(ISystem*, system, systems)
	{
		(*system)->Update();
	}
}

void Engine::AddSystem(ISystem * system)
{
	systems.Add(system);
}
