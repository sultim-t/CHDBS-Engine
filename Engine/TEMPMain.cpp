#include <Engine/Rendering/OpenGL.h>

#define GLEW_STATIC
#include <GLEW\glew.h>

#include <Engine/Rendering/Mesh.h>
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

#include <Engine/Engine.h>
#include <Engine/Systems/RenderingSystem.h>

#include <Engine/TEMP/model.h>


float *vertices;

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

	Shader shader = Shader();
	Shader simpleDepthShader = Shader();

	shader.Load("TEMP/3.1.3.shadow_mapping.vs.txt", "TEMP/3.1.3.shadow_mapping.fs.txt");
	simpleDepthShader.Load("TEMP/3.1.3.shadow_mapping_depth.vs.txt", "TEMP/3.1.3.shadow_mapping_depth.fs.txt");

	float v[] = {
		// back face
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
		1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
		1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
															  // front face
															  -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
															  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
															  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
															  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
															  -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
															  -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
																													// left face
																													-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
																													-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
																													-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
																													-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
																													-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
																													-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
																																										  // right face
																																										  1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
																																										  1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
																																										  1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
																																										  1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
																																										  1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
																																										  1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
																																																							   // bottom face
																																																							   -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
																																																							   1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
																																																							   1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
																																																							   1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
																																																							   -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
																																																							   -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
																																																																					 // top face
																																																																					 -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
																																																																					 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
																																																																					 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
																																																																					 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
																																																																					 -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
																																																																					 -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
	};

	vertices = v;
	

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

	Vector3 lightPosN = Vector3(-2.0f, 4.0f, -1.0f);


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

	ModelLE doubleBarrel("TEMP/DoubleBarrel/DoubleBarrel.obj");
	ModelLE terrain("TEMP/DoubleBarrel/beacj.fbx");

	Texture textureDB = Texture();
	textureDB.Load("TEMP/DoubleBarrel/WeaponsPalette.png");
	//Texture terrainTexture = Texture();
	//terrainTexture.Load("TEMP/DoubleBarrel/TerrainPalette.png");

	Transform dbTransform = Transform();
	dbTransform.Translate(Vector3(0, 0, -10));
	dbTransform.Rotate(Vector3(90, 0, 0));

	Transform terrainTransform = Transform();
	terrainTransform.Translate(Vector3(-20, 26, 20));
	terrainTransform.Rotate(Vector3(-90, -90, 0));










	CModel cmodel = CModel();
	cmodel.Enable();
	cmodel.Load("TEMP/DoubleBarrel/DoubleBarrel.obj");

	Material mat = Material({ textureDB });
	mat.BindShader(shader);
	
	cmodel.meshes[0].BindMaterial(mat);







	while (!ContextWindow::Instance().ShouldClose())
	{
		Time::Calculate();

		// update logic, MUST BE IN ENGINE
		cameraEntity->GetComponent<CFreeMovement>()->Update();
		cameraEntity->GetComponent<CCamera>()->Update();

		// cube4.GetTransform().Rotate(Vector3(Time::GetDeltaTime() * 30, Time::GetDeltaTime() * 40, Time::GetDeltaTime() * 20));

		// render
		glClearColor(0.5f, 0.85f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 1. render depth of scene to texture (from light's perspective)
		Matrix4 lightProjM, lightViewM, lightSpaceM;
		lightProjM = Projection::Ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100);
		lightViewM = Transform::LookAt(lightPosN, Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
		lightSpaceM = lightViewM * lightProjM;
		simpleDepthShader.Use();
		simpleDepthShader.SetMat4("lightSpaceMatrix", lightSpaceM);

		glViewport(0, 0, shadowTexture.GetWidth(), shadowTexture.GetHeight());
		glBindFramebuffer(GL_FRAMEBUFFER, shadowTexture.GetFBO());
		glClear(GL_DEPTH_BUFFER_BIT);
		
		// ---------------------
		//texture.Activate(0);
		// simpleDepthShader.SetMat4("model", cube4.GetTransform().GetTransformMatrix());
		// cube4.GetComponent<CModel>()->Draw(simpleDepthShader);

		simpleDepthShader.SetMat4("model", terrainTransform.GetTransformMatrix());
		terrain.Draw(simpleDepthShader.GetProgramID());

		//textureDB.Activate(0);
		simpleDepthShader.SetMat4("model", dbTransform.GetTransformMatrix());
		doubleBarrel.Draw(simpleDepthShader.GetProgramID());
		// ---------------------

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// 2. render scene as normal using the generated depth/shadow map  
		ContextWindow::Instance().ResetViewport();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.Use();


		CCamera *cam = cameraEntity->GetComponent<CCamera>();

		Matrix4 projM = cam->GetProjectionMatrix(1280, 720);
		Matrix4 viewM = cam->GetViewMatrix();
		
		shader.SetMat4("projection", projM);
		shader.SetMat4("view", viewM);
		// set light uniforms
		shader.SetVec3("viewPos", cameraEntity->GetTransform().GetPosition());
		shader.SetVec3("lightPos", lightPosN);
		shader.SetMat4("lightSpaceMatrix", lightSpaceM);

		shadowTexture.Activate(1);
		// ---------------------
		//texture.Activate(0);
		//shader.SetMat4("model", cube4.GetTransform().GetTransformMatrix());
		//cube4.GetComponent<CModel>()->Draw(shader);
		
		shader.SetMat4("model", terrainTransform.GetTransformMatrix());
		terrain.Draw(shader.GetProgramID());

		//shader.SetMat4("model", Matrix4::Matrix(1.0f, true));
		//cmodel.Draw();
		

		textureDB.Activate(0);
		shader.SetMat4("model", dbTransform.GetTransformMatrix());
		doubleBarrel.Draw(shader.GetProgramID());
		// ---------------------

		ContextWindow::Instance().SwapBuffers();
		ContextWindow::Instance().PollEvents();
	}

	ContextWindow::Instance().Terminate();
	return 0;
}