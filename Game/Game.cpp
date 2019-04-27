#include "Game.h"

#define GLEW_STATIC
#include <GLEW\glew.h>

#include <Engine/Rendering/Texture.h>
#include <Engine/Rendering/FramebufferTexture.h>
#include <Engine/Math/Vector.h>
#include <Engine/Math/Matrix.h>
#include <Engine/Math/Projection.h>
#include <Engine/Math/Transform.h>
#include <Engine/Rendering/Shader.h>
#include <Engine/Rendering/ContextWindow.h>
#include <Engine/Input/Input.h>
#include <Engine/Entities/Entity.h>
#include <Engine/Entities/EntityFactory.h>
#include <Engine/Components/CCamera.h>
#include <Engine/Components/CFreeMovement.h>
#include <Engine/Components/CModel.h>
#include <Engine/DataStructures/HashTable.h>
#include <Engine/DataStructures/DynamicArray.h>
#include <Engine/Rendering/Cubemap.h>
#include <Engine/Components/CLight.h>
#include <Engine/Rendering/Skybox.h>
#include <Engine/DataStructures/Array.h>
#include <Engine/Math/Intersection.h>
#include <Engine/Math/Ray.h>
#include <Engine/Math/AABB.h>
#include <Engine/Physics/MeshCollider.h>
#include <Engine/Physics/AABBCollider.h>
#include <Engine/Physics/SphereCollider.h>
#include <Engine/Rendering/Materials/StandardMaterial.h>
#include <Engine/Rendering/DebugDrawer.h>
#include <Engine/Physics/Rigidbody.h>
#include <Engine/Base/Event.h>


#include <Engine/Systems/RenderingSystem.h>
#include <Engine/Systems/ComponentSystem.h>
#include <Engine/Systems/PhysicsSystem.h>
#include <Engine/Systems/InputSystem.h>
#include <Engine/ResourceManager/ResourceManager.h>
#include <Engine/SceneManager/SceneManager.h>
#include <Engine/Entities/EntityFactory.h>

#include "Scripts/Weapon.h"

void Game::Start()
{
	// init engine
	engine.Init();

	// init debug drawer
	DebugDrawer::Instance().Init("Shaders/DebugDraw.vs", "Shaders/DebugDraw.fs");
}

void Game::LoadScenes()
{
}

int main()
{
	//Engine engine = Engine();
	//engine.Init();

	ContextWindow::Instance().Init("Engine", 1280, 720);

	ResourceManager::Instance().Init();
	SceneManager::Instance().Init();
	RenderingSystem::Instance().Init();
	ComponentSystem::Instance().Init();
	PhysicsSystem::Instance().Init();
	InputSystem::Instance().Init();

	DebugDrawer::Instance().Init("Shaders/DebugDraw.vs", "Shaders/DebugDraw.fs");

	EntityFactory::RegisterComponentType<CWeapon>("CWeapon");



	int sceneId = SceneManager::Instance().CreateScene("Scenes/MainScene.scene");
	//Scene &currentScene = SceneManager::Instance().GetScene(sceneId);

	SceneManager::Instance().LoadScene(sceneId);

	Entity *cameraEntity = SceneManager::Instance().GetCurrentScene().FindEntity("Camera");
	Entity *lightEntity = SceneManager::Instance().GetCurrentScene().FindEntity("Sun");
	Entity *terrainEntity = SceneManager::Instance().GetCurrentScene().FindEntity("Terrain");
	Entity *dbEntity = SceneManager::Instance().GetCurrentScene().FindEntity("Double Barrel");
	Entity *particles = SceneManager::Instance().GetCurrentScene().FindEntity("Particles");



	Array<const char*, 6> skyNames;
	skyNames[4] = "Textures/Skybox/desertsky_rt.tga";
	skyNames[5] = "Textures/Skybox/desertsky_lf.tga";
	skyNames[2] = "Textures/Skybox/desertsky_up.tga";
	skyNames[3] = "Textures/Skybox/desertsky_dn.tga";
	skyNames[0] = "Textures/Skybox/desertsky_ft.tga";
	skyNames[1] = "Textures/Skybox/desertsky_bk.tga";


	Array<const char*, 6> reflName;
	reflName[0] = "Textures/chrome.jpg";
	reflName[1] = "Textures/chrome.jpg";
	reflName[2] = "Textures/chrome.jpg";
	reflName[3] = "Textures/chrome.jpg";
	reflName[4] = "Textures/chrome.jpg";
	reflName[5] = "Textures/chrome.jpg";

	Cubemap sky = Cubemap();
	sky.LoadCubemap(skyNames);
	Skybox::Instance().BindCubemap(sky);

	Cubemap reflection = Cubemap();
	reflection.LoadCubemap(reflName);

	Shader shader = Shader();
	shader.Load("Shaders/ShadowMapped.vs", "Shaders/ShadowMapped.fs");

	Texture textureDB = Texture();
	textureDB.Init("Textures/WeaponsPalette.png");
	Texture textureTR = Texture();
	textureTR.Init("Textures/Sand0.jpg");

	StandardMaterial mat = StandardMaterial();
	StandardMaterial matTR = StandardMaterial();

	mat.Init();
	matTR.Init();

	mat.AddTexture(&textureDB);
	mat.AddTexture(&reflection);
	matTR.AddTexture(&textureTR);
	//matTR.AddTexture(&reflection);

	mat.BindShader(shader);
	matTR.BindShader(shader);

	mat.InitUniformLocations();
	matTR.InitUniformLocations();


	dbEntity->GetComponent<CModel>()->modelResource->SetMaterials(&mat);
	terrainEntity->GetComponent<CModel>()->modelResource->SetMaterials(&matTR);
	cameraEntity->GetComponent<CModel>()->modelResource->SetMaterials(&mat);

	// Recalculate time, there shouldn't be counted initialization time
	Time::Init();

	while (!ContextWindow::Instance().ShouldClose())
	{
		Time::Calculate();

		while (Time::ToFixedUpdate())
		{
			PhysicsSystem::Instance().Update();
			ComponentSystem::Instance().FixedUpdate();
		}

		InputSystem::Instance().Update();
		ComponentSystem::Instance().Update();
		RenderingSystem::Instance().Update();
	}

	ContextWindow::Instance().Terminate();
	return 0;
}