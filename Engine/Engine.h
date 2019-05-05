#pragma once
#include <Engine/ResourceManager/GlobalSettings.h>

class Engine
{
private:
	const GlobalSettings *settings;
	bool isActive;

private:
	// To update all systems
	void Update();
	// To update fixed update systems
	void FixedUpdate();
	// Calls Update function
	void MainLoop();

public:
	Engine();
	~Engine();
	
	// Init using settings file
	void Init(const char *globalSettingsPath);
	// Load first scene and start the main loop
	void Start();

	const GlobalSettings &GetSettings() const;
};