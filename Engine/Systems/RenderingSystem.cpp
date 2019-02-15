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
	lastMeshId = lastMaterialId
		= lastShaderId = lastTextureId = 0;
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

	matMeshes.Init(32, 8);
	matTextures.Init(32, 8);
	matShaders.Init(32, 8);

	shadowMap = FramebufferTexture();
	shadowMap.Create(1024, 1024);
	shadowMap.SetType(TEXTURE_TYPE_SHADOWMAP);

	depthShader.Load("Systems/ShadowMapping.vs", "Systems/ShadowMapping.fs");
	Skybox::Instance().Init();
}

void RenderingSystem::Update()
{
	glClearColor(0.5f, 0.85f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	FOREACHLINKEDLIST(CCamera*, camPtr, cameras)
	{
		CCamera *cam = *camPtr;

		Matrix4 projM = cam->GetProjectionMatrix(
			(float)ContextWindow::Instance().GetWidth(),
			(float)ContextWindow::Instance().GetHeight());
		Matrix4 viewM = cam->GetViewMatrix();

		Matrix4 camSpace = viewM * projM;

		FOREACHLINKEDLIST(CLight*, lightPtr, lights)
		{
			CLight *light = *lightPtr;
			CreateShadowMap(*light, shadowMap);
			shadowMap.Activate(TEXTURE_TYPE_SHADOWMAP);

			FOREACHLINKEDLIST(CModel*, modelPtr, allModels)
			{
				CModel *model = *modelPtr;

				for (const Mesh &m : model->GetMeshes())
				{
				 	const Shader *shader = &m.GetMaterial().GetShader();
					shader->Use();

					shader->SetInt(TEXTURE_NAME_SHADOWMAP, TEXTURE_TYPE_SHADOWMAP);

					if (shader->Is3D())
					{
						shader->SetMat4("spaceM", camSpace);
						shader->SetVec3("viewPos", cam->GetPosition());
					}

					if (shader->IsAffectedByLight())
					{
						shader->SetVec3("lightPos", light->GetPosition());
						shader->SetMat4("lightSpaceMatrix", light->GetLightSpace());
					}

					m.ActivateMaterial(model->GetOwner().GetTransform().GetTransformMatrix());
					m.Draw();
				}
			}
		}
		
		Matrix4 skyCamSpace = viewM;
		// reset position
		// to make skybox feel infinitely far
		skyCamSpace.SetRow(3, Vector4(0.0f));
		skyCamSpace *= projM;
		Skybox::Instance().Draw(skyCamSpace);
	}

	ContextWindow::Instance().SwapBuffers();
	ContextWindow::Instance().PollEvents();
}

void RenderingSystem::CreateShadowMap(const CLight &light, FramebufferTexture &shadowMap)
{
	glViewport(0, 0, shadowMap.GetWidth(), shadowMap.GetHeight());
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMap.GetFBO());
	glClear(GL_DEPTH_BUFFER_BIT);

	depthShader.Use();
	depthShader.SetMat4("lightSpaceMatrix", light.GetLightSpace());
	
	// draw each model
	FOREACHLINKEDLIST(CModel*, modelPtr, allModels)
	{
		CModel *model = *modelPtr;

		FOREACHLINKEDLIST(CModel*, modelPtr, allModels)
		{
			CModel *model = *modelPtr;

			for (const Mesh &m : model->GetMeshes())
			{
				// manually set transformation
				depthShader.SetMat4("model", model->GetOwner().GetTransform().GetTransformMatrix());
				// draw without binding its material	
				m.Draw();
			}
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

	allShaders.Add(shader);
}

void RenderingSystem::Register(Mesh *mesh, const Material &material)
{
	LinkedList<Mesh*> *list = GetMatMesh(material.materialId);
	list->Add(mesh);
}

void RenderingSystem::Register(CModel * model)
{
	allModels.Add(model);
}

void RenderingSystem::Register(CLight * light)
{
	lights.Add(light);
}

void RenderingSystem::Register(CCamera * camera)
{
	cameras.Add(camera);
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

const LinkedList<Mesh*> *RenderingSystem::GetMatMesh(MaterialID id) const
{
	LinkedList<Mesh*> *result;
	matMeshes.Find(id, result);

	return result;
}

inline LinkedList<Mesh*>* RenderingSystem::GetMatMesh(MaterialID id)
{
	LinkedList<Mesh*> *result;
	matMeshes.Find(id, result);

	return result;
}
