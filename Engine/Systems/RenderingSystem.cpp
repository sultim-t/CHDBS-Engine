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
	shadowMap.Create(8192, 8192);
	shadowMap.SetType(TextureType::Shadowmap);

	depthShader.Load("Shaders/ShadowMapping.vs", "Shaders/ShadowMapping.fs");

	Skybox::Instance().Init();
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

		// get frustum for for shadowmapping
		const Frustum &frustumForShadowmap = cam->GetFrustum(0.1f, 40.0f);

		if (lights->operator[](0)->IsCastingShadows())
		{
			CreateShadowMap(*lights->operator[](0), frustumForShadowmap, shadowMap);
			shadowMap.Activate((int)TextureType::Shadowmap);
		}

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

				if (!Intersection::SphereInsideFrustum(frustum, transformedSphere))
				{
					continue;
				}

				// DebugDrawer::Instance().Draw(transformedSphere);

				StandardMaterial *mat = (StandardMaterial*)materials[j];

				const Shader &shader = mat->GetShader();

				mat->Use();
				// for (int l = 0; l < lights->GetSize(); l++)
				{
					CLight *light = lights->operator[](0);

					if (light->IsCastingShadows())
					{
						mat->ActivateShadowMap();
					}

					mat->SetLightDirection(-light->GetOwner().GetTransform().GetForward());
					mat->SetLightSpace(light->GetLightSpace(frustumForShadowmap));
				}

				mat->SetCameraSpace(camSpace);
				mat->SetCameraPosition(cam->GetPosition());

				// bind tranform
				mat->SetModel(meshesTranforms[j]);
				mat->ActivateTextures();

				// draw
				DrawMesh(vaos[j], modelMeshes[j]->GetIndices().GetSize());
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

void RenderingSystem::CreateShadowMap(const CLight &light, const Frustum &frustumForShadowmap, FramebufferTexture &shadowMap)
{
	// if not registered
	if (cameras == nullptr || lights == nullptr || models == nullptr || particleSystems == nullptr)
	{
		return;
	}

	// bind viewport for shadowmap size
	glViewport(0, 0, shadowMap.GetWidth(), shadowMap.GetHeight());

	// bind
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMap.GetFBO());

	// clear depth
	glClear(GL_DEPTH_BUFFER_BIT);

	// activate depth shader
	depthShader.Use();

	Matrix4 lightSpace = light.GetLightSpace(frustumForShadowmap);
	
	// draw each model
	for (int m = 0; m < models->GetSize(); m++)
	{
		CModel *model = models->operator[](m);

		if (!model->IsCastingShadows)
		{
			continue;
		}

		// get all meshes
		const StaticArray<MeshResource*> &modelMeshes = model->GetMeshes();
		// get their vaos
		const StaticArray<UINT> &vaos = model->GetVAO();
		
		// recalculate tranforms relative to light space
		auto &meshesTranforms = model->GetTranforms(lightSpace);

		UINT count = modelMeshes.GetSize();
		for (UINT i = 0; i < count; i++)
		{
			// set transformation
			depthShader.SetMat4("MVP", meshesTranforms[i]);
			
			// draw
			DrawMesh(vaos[i], modelMeshes[i]->GetIndices().GetSize());
		}
	}

	depthShader.Stop();

	// unbind framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// reset viewport
	ContextWindow::Instance().ResetViewport();
}

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

void RenderingSystem::Reset()
{
	this->cameras			= nullptr;
	this->lights			= nullptr;
	this->models			= nullptr;
	this->particleSystems	= nullptr;
}