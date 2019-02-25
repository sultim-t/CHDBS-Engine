#pragma once

class Engine
{
private:
	bool isActive;

public:
	Engine();
	~Engine();

	void Init();
	// Calls Update function
	void MainLoop();

private:
	// To update all systems
	void Update();
	// To update fixed update systems
	void FixedUpdate();
};