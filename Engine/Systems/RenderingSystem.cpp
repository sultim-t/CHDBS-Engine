#include "RenderingSystem.h"
#include <Engine/Rendering/FramebufferTexture.h>
#include <Engine/Rendering/OpenGL.h>
#include <Engine/Rendering/ContextWindow.h>
#include <Engine/Components/CCamera.h>
#include <Engine/Components/CModel.h>
#include <Engine/Rendering/OpenGL.h>
#include <Engine/Rendering/Skybox.h>

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

		cam->SetAspect((float)ContextWindow::Instance().GetWidth(), (float)ContextWindow::Instance().GetHeight());
		Matrix4 projM = cam->GetProjectionMatrix();
		Matrix4 viewM = cam->GetViewMatrix();

		Matrix4 camSpace = viewM * projM;

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

				for (UINT k = 0; k < model->GetMeshes().GetSize(); k++)
				{
					const Shader &shader = model->GetMaterials()[k]->GetShader();
					shader.Use();

					if (light->IsCastingShadows())
					{
						shader.SetInt(TEXTURE_NAME_SHADOWMAP, (int)TextureType::Shadowmap);
					}

					if (shader.Is3D())
					{
						shader.SetMat4("spaceM", camSpace);
						shader.SetVec3("viewPos", cam->GetPosition());
					}

					if (shader.IsAffectedByLight())
					{
						shader.SetVec3("lightDir", -light->GetOwner().GetTransform().GetForward());
						shader.SetMat4("lightSpaceMatrix", light->GetLightSpace());
					}

					// todo: delete mesh class, import its functions to modelreosurce

					model->GetMaterials()[k]->
					model->GetMeshes()[k]->PrepareMaterial(model->GetOwner().GetTransform().GetTransformMatrix());
					model->GetMeshes()[k]->ActivateMaterial();
					model->GetMeshes()[k]->Draw();
				}
			}
		}
		
		Matrix4 skyCamSpace = viewM;
		// reset position
		// to make skybox feel infinitely far
		skyCamSpace.SetRow(3, Vector4(0.0f));
		skyCamSpace *= projM;
		Skybox::Instance().Draw(skyCamSpace);
	
		for (int p = 0; p < particleSystems.GetSize(); p++)
		{
			CParticleSystem *ps = particleSystems[p];

			ps->BindCamera(cam);
			ps->Render();
		}
	}

	ContextWindow::Instance().SwapBuffers();
	ContextWindow::Instance().PollEvents();
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

		Matrix4 modelWorldSpace = model->GetOwner().GetTransform().GetTransformMatrix() * lightSpace;

		for (const Mesh &m : model->GetMeshes())
		{
			// manually set transformation
			depthShader.SetMat4("MVP", m.GetTransform().GetTransformMatrix() * modelWorldSpace);
			
			// draw without binding its material
			m.Draw();
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

void RenderingSystem::Register(Mesh *mesh)
{
	mesh->meshId = lastMeshId;
	meshes.Add(lastMeshId, mesh);
	lastMeshId++;
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
