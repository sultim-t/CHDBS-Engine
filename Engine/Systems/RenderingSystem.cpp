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

RenderingSystem::RenderingSystem()
{
	lastMeshId = lastMaterialId	= lastShaderId = lastTextureId = 0;
}

RenderingSystem::~RenderingSystem()
{ }

void RenderingSystem::Init()
{
	// init data structures
	meshes.Init(32, 16);
	materials.Init(32, 8);
	textures.Init(64, 8);
	shaders.Init(32, 8);

	meshes.DeclareHashFunction(HashUnsigned);
	materials.DeclareHashFunction(HashUnsigned);
	textures.DeclareHashFunction(HashUnsigned);
	shaders.DeclareHashFunction(HashUnsigned);

	matTextures.Init(32, 8);
	matShaders.Init(32, 8);

	cameras.Init(8);
	lights.Init(8);
	allShaders.Init(16);
	allModels.Init(64);
	particleSystems.Init(64);

	shadowMap = FramebufferTexture();
	shadowMap.Create(1024, 1024);
	shadowMap.SetType(TextureType::Shadowmap);

	depthShader.Load("Shaders/ShadowMapping.vs", "Shaders/ShadowMapping.fs");

	Skybox::Instance().Init();
}

void RenderingSystem::Update()
{
	glClearColor(0.5f, 0.85f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i < cameras.GetSize(); i++)
	{
		glClear(GL_DEPTH_BUFFER_BIT);

		CCamera *cam = cameras[i];

		// reset aspect
		cam->SetAspect((float)ContextWindow::Instance().GetWidth(), (float)ContextWindow::Instance().GetHeight());
		
		// caluclate matrices
		Matrix4 projM = cam->GetProjectionMatrix();
		Matrix4 viewM = cam->GetViewMatrix();

		// calculate camera space matrix
		Matrix4 camSpace = viewM * projM;
		
		// get frustum for culling
		Frustum frustum = cam->GetFrustum();

		DebugDrawer::Instance().Draw(frustum);

		for (int l = 0; l < lights.GetSize(); l++)
		{
			CLight *light = lights[l];

			if (light->IsCastingShadows())
			{
				CreateShadowMap(light->GetLightSpace(), shadowMap);
				shadowMap.Activate((int)TextureType::Shadowmap);
			}

			for (int m = 0; m < allModels.GetSize(); m++)
			{
				CModel *model = allModels[m];

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
						//Logger::Print(model->path);
						continue;
					}

					StandardMaterial *mat = (StandardMaterial*)materials[j];

					const Shader &shader = mat->GetShader();

					mat->Use();

					if (light->IsCastingShadows())
					{
						mat->ActivateShadowMap();
					}

					mat->SetCameraSpace(camSpace);
					mat->SetCameraPosition(cam->GetPosition());

					mat->SetLightDirection(-light->GetOwner().GetTransform().GetForward());
					mat->SetLightSpace(light->GetLightSpace());

					// bind tranform
					mat->SetModel(meshesTranforms[j]);
					mat->ActivateTextures();

					// draw
					DrawMesh(vaos[j], modelMeshes[j]->GetIndices().GetSize());
				}
			}
		}
		
		// Render skybox
		Matrix4 skyCamSpace = viewM;
		// reset position
		// to make skybox feel infinitely far
		skyCamSpace.SetRow(3, Vector4(0.0f));
		skyCamSpace *= projM;
		Skybox::Instance().Draw(skyCamSpace);
	
		// Render all particle systems
		for (int p = 0; p < particleSystems.GetSize(); p++)
		{
			CParticleSystem *ps = particleSystems[p];

			ps->BindCamera(cam);
			ps->Render();
		}

		// Render debug objects
		DebugDrawer::Instance().BindSpaceMatrix(camSpace);
		DebugDrawer::Instance().DrawQueues();
	}

	ContextWindow::Instance().SwapBuffers();
	ContextWindow::Instance().PollEvents();
}

void RenderingSystem::DrawMesh(UINT vao, UINT indicesCount)
{
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void RenderingSystem::CreateShadowMap(const Matrix4 &lightSpace, FramebufferTexture &shadowMap)
{
	glViewport(0, 0, shadowMap.GetWidth(), shadowMap.GetHeight());
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMap.GetFBO());
	glClear(GL_DEPTH_BUFFER_BIT);

	depthShader.Use();
	
	// draw each model
	for (int m = 0; m < allModels.GetSize(); m++)
	{
		CModel *model = allModels[m];

		if (!model->IsCastingShadows)
		{
			continue;
		}

		// get meshes
		auto &modelMeshes = model->GetMeshes();
		auto &vaos = model->GetVAO();
		
		// recalculate tranforms relative to light
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

void RenderingSystem::Register(Material *material)
{
	material->materialId = lastMaterialId;
	materials.Add(lastMaterialId, material);
	lastMaterialId++;
}

void RenderingSystem::Register(ITexture *texture)
{
	texture->textureId = lastTextureId;
	textures.Add(lastTextureId, texture);
	lastTextureId++;
}

void RenderingSystem::Register(Shader *shader)
{
	shader->shaderId = lastShaderId;
	shaders.Add(lastShaderId, shader);
	lastShaderId++;

	allShaders.Push(shader);
}

void RenderingSystem::Register(CModel * model)
{
	allModels.Push(model);
}

void RenderingSystem::Register(CLight * light)
{
	lights.Push(light);
}

void RenderingSystem::Register(CCamera * camera)
{
	cameras.Push(camera);
}

void RenderingSystem::Register(CParticleSystem * ps)
{
	particleSystems.Push(ps);
}

Mesh *RenderingSystem::GetMesh(MeshID id) const
{
	Mesh *result;
	meshes.Find(id, result);

	return result;
}

Material *RenderingSystem::GetMaterial(MaterialID id) const
{
	Material *result;
	materials.Find(id, result);

	return result;
}

ITexture *RenderingSystem::GetTexture(TextureID id) const
{
	ITexture *result;
	textures.Find(id, result);

	return result;
}

Shader *RenderingSystem::GetShader(ShaderID id) const
{
	Shader *result;
	shaders.Find(id, result);

	return result;
}
