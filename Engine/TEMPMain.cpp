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

#include <Engine/Engine.h>
#include <Engine/Systems/RenderingSystem.h>
#include <Engine/Systems/ComponentSystem.h>

int main()
{
	//Engine *engine = new Engine();
	//engine->Init();

	//RenderingSystem *rendering = new RenderingSystem();
	//rendering->Init();

	//engine->AddSystem(rendering);

	//engine->MainLoop();

	//return 0;

	ContextWindow::Instance().Init("Engine", 1280, 720);
	RenderingSystem::Instance().Init();
	ComponentSystem::Instance().Init();

	Shader shader = Shader();
	shader.Init();
	shader.Load("TEMP/3.1.3.shadow_mapping.vs.txt", "TEMP/3.1.3.shadow_mapping.fs.txt");
	// shader configuration
	shader.Use();
	shader.SetInt("diffuseTexture", 0);
	shader.SetInt("shadowMap", 1);

	// From XML
	Entity *cameraEntity = EntityFactory::Instance().CreateEntity("entityTest.xml");	
	Entity *lightEntity = EntityFactory::Instance().CreateEntity("light.xml");
	Entity *terrainEntity = EntityFactory::Instance().CreateEntity("terrain.xml");
	Entity *dbEntity = EntityFactory::Instance().CreateEntity("doubleBarrel.xml");


	Texture textureDB = Texture();
	textureDB.Load("TEMP/DoubleBarrel/WeaponsPalette.png");
	Material mat = Material({ textureDB });
	mat.BindShader(shader);
	
	dbEntity->GetComponent<CModel>()->
		meshes[0].BindMaterial(mat);

	terrainEntity->GetComponent<CModel>()->
		meshes[0].GetMaterial().
		BindShader(shader);


	while (!ContextWindow::Instance().ShouldClose())
	{
		Time::Calculate();

		ComponentSystem::Instance().Update();
		RenderingSystem::Instance().Update();
	}

	ContextWindow::Instance().Terminate();
	return 0;
}