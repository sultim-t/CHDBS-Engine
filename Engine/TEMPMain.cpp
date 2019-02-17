#include <Engine/Rendering/OpenGL.h>

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

#include <Engine/Engine.h>
#include <Engine/Systems/RenderingSystem.h>
#include <Engine/Systems/ComponentSystem.h>

int main()
{
	//Engine engine = Engine();
	//engine.Init();

	ContextWindow::Instance().Init("Engine", 1280, 720);
	RenderingSystem::Instance().Init();
	ComponentSystem::Instance().Init();


	StaticArray<const char*, 6> skyNames;
	skyNames[0] = "TEMP/DoubleBarrel/Skybox/right.jpg";
	skyNames[1] = "TEMP/DoubleBarrel/Skybox/left.jpg";
	skyNames[2] = "TEMP/DoubleBarrel/Skybox/top.jpg";
	skyNames[3] = "TEMP/DoubleBarrel/Skybox/bottom.jpg";
	skyNames[4] = "TEMP/DoubleBarrel/Skybox/front.jpg";
	skyNames[5] = "TEMP/DoubleBarrel/Skybox/back.jpg";

	StaticArray<const char*, 6> reflName;
	reflName[0] = "TEMP/DoubleBarrel/chrome.jpg";
	reflName[1] = "TEMP/DoubleBarrel/chrome.jpg";
	reflName[2] = "TEMP/DoubleBarrel/chrome.jpg";
	reflName[3] = "TEMP/DoubleBarrel/chrome.jpg";
	reflName[4] = "TEMP/DoubleBarrel/chrome.jpg";
	reflName[5] = "TEMP/DoubleBarrel/chrome.jpg";

	Cubemap sky = Cubemap();
	sky.LoadCubemap(skyNames);
	Skybox::Instance().BindCubemap(sky);

	Cubemap reflection = Cubemap();
	reflection.LoadCubemap(reflName);

	Shader shader = Shader();
	shader.Init();
	shader.Load("TEMP/3.1.3.shadow_mapping.vs.txt", "TEMP/3.1.3.shadow_mapping.fs.txt");

	// From XML
	Entity *cameraEntity = EntityFactory::CreateEntity("entityTest.xml");	
	Entity *lightEntity = EntityFactory::CreateEntity("light.xml");
	Entity *terrainEntity = EntityFactory::CreateEntity("terrain.xml");
	Entity *dbEntity = EntityFactory::CreateEntity("doubleBarrel.xml");
	Entity *particles = EntityFactory::CreateEntity("particleSystem.xml");


	Texture textureDB = Texture();
	textureDB.Load("TEMP/DoubleBarrel/WeaponsPalette.png");
	Material mat = Material({ textureDB, reflection });
	mat.BindShader(shader);
	
	dbEntity->GetComponent<CModel>()->meshes[0].
		BindMaterial(mat);

	for (UINT i = 0; i < terrainEntity->GetComponent<CModel>()->meshes.size(); i++)
	{
		terrainEntity->GetComponent<CModel>()->meshes[i].GetMaterial().
			BindShader(shader);
		terrainEntity->GetComponent<CModel>()->meshes[i].GetMaterial().
			AddTexture(reflection);
	}

	// Recalculate time, there shouldn't be counted initialization time
	Time::Calculate();

	while (!ContextWindow::Instance().ShouldClose())
	{
		Time::Calculate();

		ComponentSystem::Instance().Update();
		RenderingSystem::Instance().Update();
	}

	ContextWindow::Instance().Terminate();
	return 0;
}