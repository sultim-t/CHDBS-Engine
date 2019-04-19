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

#include <Engine/Systems/RenderingSystem.h>
#include <Engine/Systems/ComponentSystem.h>
#include <Engine/Systems/PhysicsSystem.h>
#include <Engine/Systems/InputSystem.h>
#include <Engine/ResourceManager/ResourceManager.h>
#include <Engine/SceneManager/SceneManager.h>

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


	// From XML
	//Entity *cameraEntity = EntityFactory::CreateEntity("Prefabs/camera.xml");
	//Entity *lightEntity = EntityFactory::CreateEntity("Prefabs/light.xml");
	//Entity *terrainEntity = EntityFactory::CreateEntity("Prefabs/terrain.xml");
	//Entity *dbEntity = EntityFactory::CreateEntity("Prefabs/doubleBarrel.xml");
	//Entity *particles = EntityFactory::CreateEntity("Prefabs/particleSystem.xml");

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

	
	/*Material mat = Material({ textureDB, reflection });
	mat.BindShader(shader);
	Material matTR = Material({ textureTR, reflection });
	matTR.BindShader(shader);*/

	StandardMaterial mat = StandardMaterial();
	StandardMaterial matTR = StandardMaterial();

	mat.Init();
	matTR.Init();

	mat.AddTexture(&textureDB);
	mat.AddTexture(&reflection);
	matTR.AddTexture(&textureTR);
	matTR.AddTexture(&reflection);

	mat.BindShader(shader);
	matTR.BindShader(shader);

	mat.InitUniformLocations();
	matTR.InitUniformLocations();


	dbEntity->GetComponent<CModel>()->modelResource->SetMaterials(&mat);
	terrainEntity->GetComponent<CModel>()->modelResource->SetMaterials(&matTR);
	cameraEntity->GetComponent<CModel>()->modelResource->SetMaterials(&mat);

	//cameraEntity->GetComponent<CModel>()->meshes[i].GetTransform().Translate(Vector3(0, 0, -0.2f));
	//cameraEntity->GetComponent<CModel>()->meshes[i].GetTransform().SetRotation(Vector3(0, -90, -90));
	//cameraEntity->GetComponent<CModel>()->meshes[i].GetTransform().SetScale(Vector3(3, -3, 3));

	// Recalculate time, there shouldn't be counted initialization time
	Time::Init();

	//auto &meshes = terrainEntity->GetComponent<CModel>()->GetMeshes();

	//StaticArray<StaticArray<Triangle>*> meshtr;
	//meshtr.Init(meshes.GetSize());
	//auto &transformsArr = terrainEntity->GetComponent<CModel>()->GetTranforms();

	//for (UINT j = 0; j < meshes.GetSize(); j++)
	//{
	//	MeshColliderResource res = MeshColliderResource(meshes[j]->GetTriangles());

	//	StaticArray<Triangle> *tr = new StaticArray<Triangle>();
	//	tr->Init(meshes[j]->GetTriangles().GetSize());

	//	res.TransformCollider(transformsArr[j], *tr);

	//	meshtr[j] = tr;

	//	MeshCollider *col = new MeshCollider(meshtr[j]);
	//	//PhysicsSystem::Instance().Register(col);
	//}

	//AABBCollider c = AABBCollider(AABB(Vector3(-100,-10,-100), Vector3(100,0,100)));
	//Transform setd = Transform();
	//c.SetTransform(&setd);
	//PhysicsSystem::Instance().Register(&c);

	//float s = 0;

	while (!ContextWindow::Instance().ShouldClose())
	{
		Time::Calculate();

		while (Time::ToFixedUpdate())
		{
			PhysicsSystem::Instance().Update();
		}

		InputSystem::Instance().Update();
		ComponentSystem::Instance().Update();
		RenderingSystem::Instance().Update();
		
		//const SphereCollider &dbCollider = (const SphereCollider&)(dbEntity->GetComponent<Rigidbody>()->GetCollider());
		////DebugDrawer::Instance().Draw(dbCollider.GetSphere());

		//s += Time::GetDeltaTime();
		//if (Input::IsPressed(Keycode::KeyF) && s > 0.5f)
		//{
		//	for (int i = 0; i < 7; i++)
		//	{
		//		Vector3 point, normal;

		//		Vector3 localDir = Vector3((i - 3) / 3.0f * 0.2f, (i % 2 - 0.5f) * 0.1f, 1);
		//		Vector3 dir = cameraEntity->GetTransform().DirectionFromLocal(localDir);
		//		Ray ray = Ray(cameraEntity->GetTransform().GetPosition(), dir);

		//		if (Intersection::RaySphere(ray, dbEntity->GetComponent<Rigidbody>()->GetCollider().GetBoundingSphere(), point, normal))
		//		{
		//			particles->GetTransform().SetPosition(point);
		//			particles->GetComponent<CParticleSystem>()->Emit(15, normal);

		//			dbEntity->GetComponent<Rigidbody>()->AddImpulse(dir.GetNormalized() * 2000.0f);

		//			continue;
		//		}

		//		for (UINT j = 0; j < meshes.GetSize(); j++)
		//		{
		//			if (Intersection::MeshRay(*meshtr[j], ray, point, normal))
		//			{
		//				particles->GetTransform().SetPosition(point);
		//				particles->GetComponent<CParticleSystem>()->Emit(15, normal);

		//				break;
		//			}
		//		}
		//	}

		//	s = 0;
		//}
	}

	ContextWindow::Instance().Terminate();
	return 0;
}