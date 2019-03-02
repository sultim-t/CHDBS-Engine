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
#include <Engine/DataStructures/Array.h>
#include <Engine/Math/Intersection.h>
#include <Engine/Math/Ray.h>
#include <Engine/Math/AABB.h>
#include <Engine/Physics/MeshCollider.h>

#include <Engine/Engine.h>
#include <Engine/Systems/RenderingSystem.h>
#include <Engine/Systems/ComponentSystem.h>
#include <Engine/Systems/PhysicsSystem.h>
#include <Engine/Systems/InputSystem.h>
#include <Engine/ResourceManager/ResourceManager.h>

int main()
{
	//Engine engine = Engine();
	//engine.Init();

	ContextWindow::Instance().Init("Engine", 1280, 720);
	
	ResourceManager::Instance().Init();
	RenderingSystem::Instance().Init();
	ComponentSystem::Instance().Init();
	PhysicsSystem::Instance().Init();
	InputSystem::Instance().Init();

	Array<const char*, 6> skyNames;
	skyNames[4] = "TEMP/DoubleBarrel/Skybox/desertsky_rt.tga";
	skyNames[5] = "TEMP/DoubleBarrel/Skybox/desertsky_lf.tga";
	skyNames[2] = "TEMP/DoubleBarrel/Skybox/desertsky_up.tga";
	skyNames[3] = "TEMP/DoubleBarrel/Skybox/desertsky_dn.tga";
	skyNames[0] = "TEMP/DoubleBarrel/Skybox/desertsky_ft.tga";
	skyNames[1] = "TEMP/DoubleBarrel/Skybox/desertsky_bk.tga";


	Array<const char*, 6> reflName;
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
	Texture textureTR = Texture();
	textureTR.Load("TEMP/DoubleBarrel/Materials/Textures/TerrainPalette.png");

	Material mat = Material({ textureDB, reflection });
	mat.BindShader(shader);
	Material matTR = Material({ textureTR, reflection });
	matTR.BindShader(shader);

	dbEntity->GetComponent<CModel>()->meshes[0].BindMaterial(mat);

	for (UINT i = 0; i < terrainEntity->GetComponent<CModel>()->meshes.size(); i++)
	{
		terrainEntity->GetComponent<CModel>()->meshes[i].BindMaterial(matTR);
	}

	for (UINT i = 0; i < cameraEntity->GetComponent<CModel>()->meshes.size(); i++)
	{
		cameraEntity->GetComponent<CModel>()->meshes[i].BindMaterial(mat);
		cameraEntity->GetComponent<CModel>()->meshes[i].GetTransform().Translate(Vector3(0,0, -0.8f));
		cameraEntity->GetComponent<CModel>()->meshes[i].GetTransform().SetRotation(Vector3(0, -90, 0));
		cameraEntity->GetComponent<CModel>()->meshes[i].GetTransform().SetScale(Vector3(3, 3, 3));
	}


	// Recalculate time, there shouldn't be counted initialization time
	Time::Init();

	auto &meshes = terrainEntity->GetComponent<CModel>()->GetMeshes();

	StaticArray<StaticArray<Triangle>*> meshtr;
	meshtr.Init(meshes.size());

	for (UINT j = 0; j < meshes.size(); j++)
	{
		const MeshResource &r = meshes[j].GetMeshResource();

		MeshColliderResource res = MeshColliderResource(r);
		StaticArray<Triangle> *tr = new StaticArray<Triangle>();
		tr->Init(r.GetTriangles().GetSize());

		res.TransformCollider(terrainEntity->GetTransform(), *tr);

		meshtr[j] = tr;

		MeshCollider *col = new MeshCollider(meshtr[j]);
		PhysicsSystem::Instance().Register(col);
	}


	float s = 0;

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

		s += Time::GetDeltaTime();
		if (Input::IsPressed(Keycode::KeyF) && s > 0.5f)
		{
			for (int i = 0; i < 7; i++)
			{
				Vector3 point, normal;

				Vector3 dir = Quaternion(Euler(0, (i - 3) * 6.0f, (i % 2) * 6.0f)) * cameraEntity->GetTransform().GetForward();
				Ray ray = Ray(cameraEntity->GetTransform().GetPosition(), dir);

				for (UINT j = 0; j < meshes.size(); j++)
				{
					if (Intersection::MeshRay(*meshtr[j], ray, point, normal))
					{
						particles->GetTransform().SetPosition(point);
						particles->GetTransform().SetRotation(Quaternion::FromDirection(normal));

						particles->GetComponent<CParticleSystem>()->Emit(15);
					}
				}
			}

			s = 0;
		}
	}

	ContextWindow::Instance().Terminate();
	return 0;
}