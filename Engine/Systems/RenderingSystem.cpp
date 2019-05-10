#include "RenderingSystem.h"
#include <Engine/Components/CCamera.h>
#include <Engine/Components/CModel.h>
#include <Engine/Rendering/FramebufferTexture.h>
#include <Engine/Rendering/OpenGL.h>
#include <Engine/Rendering/ContextWindow.h>
#include <Engine/Rendering/OpenGL.h>
#include <Engine/Rendering/Skybox.h>
#include <Engine/Rendering/Materials/StandardMaterial.h>
#include <Engine/Rendering/DebugDrawer.h>
#include <Engine/Math/Intersection.h>
#include <Engine/ResourceManager/MeshResource.h>
#include <Engine/Rendering/GUI/TextRenderer.h>

Cubemap cubemap;

// Identity function
// Use ONLY if keys are ordered natural numbers
UINT HashUnsigned(UINT i)
{
	return i;
}

RenderingSystem::RenderingSystem() : models(nullptr), lights(nullptr), cameras(nullptr), particleSystems(nullptr)
{ }

RenderingSystem::~RenderingSystem()
{ }

void RenderingSystem::Init()
{
	//shadowMap.Create(1024, 1024);
	//shadowMap.SetType(TextureType::Shadowmap);

	//depthShader.Load("Shaders/ShadowMapping.vs", "Shaders/ShadowMapping.fs");
	
	StaticArray<String> skyReflNames;
	skyReflNames.Init(6);

	skyReflNames[0] = "Textures/SkyboxRefl/desertsky_ft.tga";
	skyReflNames[1] = "Textures/SkyboxRefl/desertsky_bk.tga";
	skyReflNames[2] = "Textures/SkyboxRefl/desertsky_up.tga";
	skyReflNames[3] = "Textures/SkyboxRefl/desertsky_dn.tga";
	skyReflNames[4] = "Textures/SkyboxRefl/desertsky_rt.tga";
	skyReflNames[5] = "Textures/SkyboxRefl/desertsky_lf.tga";

	cubemap = Cubemap();
	cubemap.LoadCubemap(skyReflNames);

	shaders.Init(8, 4);
	shaders.DeclareHashFunction(String::StringHash);
}

void RenderingSystem::Update()
{
	// if not registered
	if (cameras == nullptr || lights == nullptr || models == nullptr || particleSystems == nullptr)
	{
		return;
	}

	glClearColor(0.5f, 0.85f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < cameras->GetSize(); i++)
	{
		glClear(GL_DEPTH_BUFFER_BIT);

		CCamera *cam = cameras->operator[](i);

		// reset aspect
		cam->SetAspect((float)ContextWindow::Instance().GetWidth(), (float)ContextWindow::Instance().GetHeight());

		// caluclate matrices
		Matrix4 projM = cam->GetProjectionMatrix();
		Matrix4 viewM = cam->GetViewMatrix();

		// calculate camera space matrix
		Matrix4 camSpace = viewM * projM;

		// get frustum for culling
		const Frustum frustum = cam->GetFrustum();

		//// get frustum for for shadowmapping
		//const Frustum &frustumForShadowmap = cam->GetFrustum(0.1f, 40.0f);

		//if (lights->operator[](0)->IsCastingShadows())
		//{
		//	CreateShadowMap(*lights->operator[](0), frustumForShadowmap, shadowMap);
		//	shadowMap.Activate((int)TextureType::Shadowmap);
		//}

		for (int m = 0; m < models->GetSize(); m++)
		{
			CModel *model = models->operator[](m);

			// get meshes
			auto &modelMeshes = model->GetMeshes();
			auto &materials = model->GetMaterials();
			auto &vaos = model->GetVAO();

			// recalculate tranforms relative to global space
			auto &meshesTranforms = model->GetTranforms();

			UINT count = modelMeshes.GetSize();
			for (UINT j = 0; j < count; j++)
			{
				// get bounding sphre
				Sphere transformedSphere = modelMeshes[j]->GetBoundingSphere();
				// and move it according to its global transformation
				transformedSphere.Move(Transform::DecomposePosition(meshesTranforms[j]));
				transformedSphere.SetRadius(transformedSphere.GetRadius() * 1.3f);

				if (!Intersection::SphereInsideFrustum(frustum, transformedSphere))
				{
					continue;
				}

				// DebugDrawer::Instance().Draw(transformedSphere);

				// get mesh's material
				StandardMaterial *mat = (StandardMaterial*)materials[modelMeshes[j]->GetMaterialIndex()];

				// activate material
				mat->Use();

				// activate shadowmap (only one directional for now)
				if (lights->operator[](0)->IsCastingShadows())
				{
					mat->ActivateShadowMap();
				}

				// set light count to shader
				mat->SetLightCount(lights->GetSize());

				// for each light
				for (int l = 0; l < lights->GetSize(); l++)
				{
					// set light's properties
					CLight *light = lights->operator[](l);

					mat->SetLight(*light, l);

					//if (light->GetLightType() != LightType::Point)
					//{
					//	mat->SetLightSpace(light->GetLightSpace(frustumForShadowmap));
					//}
				}

				// set reflection cubemap
				mat->SetReflectionCubemap(&cubemap);

				// set camera options
				mat->SetCameraSpace(camSpace);
				mat->SetCameraPosition(cam->GetPosition());

				// bind tranform
				mat->SetModel(meshesTranforms[j]);

				// activate all textures
				mat->ActivateTextures();

				// draw
				DrawMesh(vaos[j], modelMeshes[j]->GetIndices().GetSize());

				// deactivate all textures
				mat->DeactivateTextures();
			}
		}

		// Render all particle systems
		for (int p = 0; p < particleSystems->GetSize(); p++)
		{
			CParticleSystem *ps = particleSystems->operator[](p);

			ps->BindCamera(cam);
			ps->Render();
		}

		// Render skybox
		DrawSkybox(viewM, projM);

		// Render debug objects
		DebugDrawer::Instance().BindSpaceMatrix(camSpace);
		DebugDrawer::Instance().DrawQueues();

		glClear(GL_DEPTH_BUFFER_BIT);

		TextRenderer::Instance().Draw("+ 100", 25, 25, 1.0f, Color3F(0, 1, 0));
		TextRenderer::Instance().Draw("2 / 100", 1100, 25, 1.0f);
	}

	ContextWindow::Instance().SwapBuffers();
	ContextWindow::Instance().PollEvents();
}

void RenderingSystem::DrawMesh(UINT vao, UINT indicesCount)
{
	// bind current vao
	glBindVertexArray(vao);

	// draw
	glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void RenderingSystem::DrawSkybox(const Matrix4 &viewMatrix, const Matrix4 &projMatrix)
{
	Matrix4 skyCamSpace(viewMatrix);

	// reset position
	// to make skybox feel infinitely far
	skyCamSpace.SetRow(3, Vector4(0.0f));

	skyCamSpace *= projMatrix;

	// draw
	Skybox::Instance().Draw(skyCamSpace);
}

//void RenderingSystem::CreateShadowMap(const CLight &light, const Frustum &frustumForShadowmap, FramebufferTexture &shadowMap)
//{
//	// if not registered
//	if (cameras == nullptr || lights == nullptr || models == nullptr || particleSystems == nullptr)
//	{
//		return;
//	}
//
//	// bind viewport for shadowmap size
//	glViewport(0, 0, shadowMap.GetWidth(), shadowMap.GetHeight());
//
//	// bind
//	glBindFramebuffer(GL_FRAMEBUFFER, shadowMap.GetFBO());
//
//	// clear depth
//	glClear(GL_DEPTH_BUFFER_BIT);
//
//	// activate depth shader
//	depthShader.Use();
//
//	Matrix4 lightSpace = light.GetLightSpace(frustumForShadowmap);
//	
//	// draw each model
//	for (int m = 0; m < models->GetSize(); m++)
//	{
//		CModel *model = models->operator[](m);
//
//		if (!model->IsCastingShadows)
//		{
//			continue;
//		}
//
//		// get all meshes
//		const StaticArray<MeshResource*> &modelMeshes = model->GetMeshes();
//		// get their vaos
//		const StaticArray<UINT> &vaos = model->GetVAO();
//		
//		// recalculate tranforms relative to light space
//		auto &meshesTranforms = model->GetTranforms(lightSpace);
//
//		UINT count = modelMeshes.GetSize();
//		for (UINT i = 0; i < count; i++)
//		{
//			// set transformation
//			depthShader.SetMat4("MVP", meshesTranforms[i]);
//			
//			// draw
//			DrawMesh(vaos[i], modelMeshes[i]->GetIndices().GetSize());
//		}
//	}
//
//	depthShader.Stop();
//
//	// unbind framebuffer
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	// reset viewport
//	ContextWindow::Instance().ResetViewport();
//}

RenderingSystem &RenderingSystem::Instance()
{
	static RenderingSystem instance;
	return instance;
}

void RenderingSystem::Register(const DynamicArray<CCamera*> *cameras)
{
	this->cameras = cameras;
}

void RenderingSystem::Register(const DynamicArray<CLight*> *lights)
{
	this->lights = lights;
}

void RenderingSystem::Register(const DynamicArray<CModel*> *models)
{
	this->models = models;
}

void RenderingSystem::Register(const DynamicArray<CParticleSystem*> *particleSystems)
{
	this->particleSystems = particleSystems;
}

const Shader *RenderingSystem::RegisterShader(const char *name, const char *vertexPath, const char *fragmentPath)
{	
	const Shader *loaded = GetShader(name);

	// if loaded
	if (loaded != nullptr)
	{
		return loaded;
	}

	// if not found, create
	Shader *result = new Shader(name);

	// compile
	result->Load(vertexPath, fragmentPath);

	// add to table
	shaders.Add(name, result);

	return result;
}

void RenderingSystem::ResetShaders()
{
	UINT count = shaders.GetSize();
	for (UINT i = 0; i < count; i++)
	{
		delete shaders[i];
	}
}

void RenderingSystem::Reset()
{
	this->cameras			= nullptr;
	this->lights			= nullptr;
	this->models			= nullptr;
	this->particleSystems	= nullptr;
}

const Shader *RenderingSystem::GetShader(const char *name) const
{
	Shader *result;

	// try to find in hash table
	if (shaders.Find(name, result))
	{
		return result;
	}

	return nullptr;
}
