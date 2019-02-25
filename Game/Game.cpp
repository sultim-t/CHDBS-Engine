#include "Game.h"
#include <Engine/Engine.h>

void main()
{
	Engine engine = Engine();

	engine.Init();
	engine.MainLoop();
}

void Game::Start()
{
}

void Game::LoadScenes()
{
}