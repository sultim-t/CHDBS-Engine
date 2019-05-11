#include "Engine.h"
#include <Engine/Systems/ISystem.h>
#include <Engine/Rendering/ContextWindow.h>
#include <Engine/Rendering/DebugDrawer.h>
#include <Engine/Rendering/GUI/TextRenderer.h>
#include <Engine/Systems/RenderingSystem.h>
#include <Engine/Systems/ComponentSystem.h>
#include <Engine/Systems/PhysicsSystem.h>
#include <Engine/Systems/InputSystem.h>
#include <Engine/ResourceManager/ResourceManager.h>
#include <Engine/SceneManager/SceneManager.h>

Engine::Engine()
{
	isActive = true;
}

Engine::~Engine()
{
	isActive = false;
}

void Engine::Init(const char *globalSettingsPath)
{
	ResourceManager::Instance().Init();

	// load settings
	settings = ResourceManager::Instance().LoadSettings(globalSettingsPath);

	// create contrext window
	ContextWindow::Instance().Init(settings->Name, settings->ScreenWidth, settings->ScreenHeight);

	// init all systems
	SceneManager::Instance().Init();
	RenderingSystem::Instance().Init();
	ComponentSystem::Instance().Init();
	PhysicsSystem::Instance().Init();
	InputSystem::Instance().Init();

	// init predefined shaders
	RenderingSystem::Instance().RegisterShader(SHADER_NAME_STANDARD, settings->StandardShaderVert, settings->StandardShaderFrag);
	RenderingSystem::Instance().RegisterShader(SHADER_NAME_DEBUG, settings->DebugShaderVert, settings->DebugShaderFrag);
	RenderingSystem::Instance().RegisterShader(SHADER_NAME_SKYBOX, settings->SkyboxShaderVert, settings->SkyboxShaderFrag);
	RenderingSystem::Instance().RegisterShader(SHADER_NAME_TEXT, settings->TextShaderVert, settings->TextShaderFrag);

	DebugDrawer::Instance().Init();
	DebugDrawer::Instance().SetShader(SHADER_NAME_DEBUG);

	Skybox::Instance().Init();
	Skybox::Instance().BindShader(SHADER_NAME_SKYBOX);

	// TODO: set of fonts in textrenderer
	// TextRenderer::Instance().Init();
}

void Engine::Start()
{
	// load first scene
	int sceneId = SceneManager::Instance().CreateScene(settings->StartScenePath);
	SceneManager::Instance().LoadScene(sceneId);

	MainLoop();
}

const GlobalSettings &Engine::GetSettings() const
{
	return *settings;
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

		isActive = !ContextWindow::Instance().ShouldClose();
	}

	// destroy
	ContextWindow::Instance().Terminate();
}

void Engine::Update()
{
	// update inputs
	InputSystem::Instance().Update();

	// update components in current scene
	ComponentSystem::Instance().Update();
	
	// render all objects in current scene
	RenderingSystem::Instance().Update();
}

void Engine::FixedUpdate()
{
	// update all physics objects
	// check their collisions, solve them
	PhysicsSystem::Instance().Update();
	ComponentSystem::Instance().FixedUpdate();
}