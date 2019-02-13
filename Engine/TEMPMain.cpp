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

	Shader shader = Shader();
	shader.Init();

	shader.Load("TEMP/3.1.3.shadow_mapping.vs.txt", "TEMP/3.1.3.shadow_mapping.fs.txt");
	
	// From XML
	Entity *cameraEntity = EntityFactory::Instance().CreateEntity("entityTest.xml");	

	// load textures
	Texture texture = Texture();
	texture.Load("Simple.bmp");
	FramebufferTexture shadowTexture = FramebufferTexture();
	shadowTexture.Create(1024, 1024);


	// shader configuration
	shader.Use();
	shader.SetInt("diffuseTexture", 0);
	shader.SetInt("shadowMap", 1);

	/*Entity cube4 = Entity();
	
	Mesh cube4Mesh = Mesh();
	//cube4Mesh.AddTexture(texture);
	//cube4Mesh.AddTexture(shadowTexture);
	cube4Mesh.InitMesh(vertices, 1152);

	CModel cube4model = CModel();
	cube4model.Enable();
	cube4model.AddMesh(cube4Mesh);

	cube4model.SetOwner(&cube4);
	cube4.AddComponent(&cube4model);
	cube4.GetTransform().SetPosition(Vector3(1, 1, 5));*/

	Entity lightEntity = Entity();
	lightEntity.GetTransform().Translate(Vector3(-2, 4, -1));
	lightEntity.GetTransform().SetRotation(Vector3(-50, 30, 0));

	CLight clight = CLight();
	lightEntity.AddComponent(&clight);
	clight.SetOwner(&lightEntity);

	clight.Enable();
	clight.IsCastingShadows() = true;
	clight.IsStatic() = true;
	clight.Init();



	

	Texture textureDB = Texture();
	textureDB.Load("TEMP/DoubleBarrel/WeaponsPalette.png");
	
	Entity dbEntity = Entity();
	dbEntity.GetTransform().Translate(Vector3(0, 0, -1));
	dbEntity.GetTransform().Rotate(Vector3(90, 0, 0));

	Entity terrainEntity = Entity();
	terrainEntity.GetTransform().Translate(Vector3(-20, 26, 20));
	terrainEntity.GetTransform().Rotate(Vector3(-90, -90, 0));

	CModel terrain = CModel();
	terrain.Enable();
	terrain.Init();
	terrain.Load("TEMP/DoubleBarrel/beacj.fbx");
	terrain.SetOwner(&terrainEntity);
	terrainEntity.AddComponent(&terrain);

	CModel cmodel = CModel();
	cmodel.Enable();
	cmodel.Init();
	cmodel.Load("TEMP/DoubleBarrel/DoubleBarrel.obj");
	cmodel.SetOwner(&dbEntity);
	dbEntity.AddComponent(&cmodel);

	Material mat = Material({ textureDB });
	mat.BindShader(shader);
	
	cmodel.meshes[0].BindMaterial(mat);
	terrain.meshes[0].GetMaterial().BindShader(shader);


	





	while (!ContextWindow::Instance().ShouldClose())
	{
		Time::Calculate();

		// update logic, MUST BE IN ENGINE
		cameraEntity->GetComponent<CFreeMovement>()->Update();
		cameraEntity->GetComponent<CCamera>()->Update();

		RenderingSystem::Instance().Update();
	}

	ContextWindow::Instance().Terminate();
	return 0;
}