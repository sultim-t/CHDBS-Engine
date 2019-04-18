#pragma once
#include <Engine/Engine.h>

class Game
{
private:
	// Engine to use
	Engine engine;

public:
	// Load scenes which are used in this game
	void LoadScenes();
	// Start game
	void Start();
};