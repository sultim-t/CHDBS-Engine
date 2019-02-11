#include "RenderingSystem.h"
#include <Engine/Rendering/FramebufferTexture.h>
#include <Engine/Rendering/OpenGL.h>
#include <Engine/Rendering/ContextWindow.h>
#include <Engine/Components/CCamera.h>

RenderingSystem *RenderingSystem::instance;


// Identity function
// Use ONLY if keys are ordered natural numbers
UINT HashUnsigned(UINT i)
{
	return i;
}
RenderingSystem::RenderingSystem()
{ 
	instance = this;
	//graphics = new GraphicsOGL();

	lastMeshId = lastMaterialId 
		= lastShaderId = lastTextureId = 0;
}

RenderingSystem::~RenderingSystem()
{
}

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
}

void RenderingSystem::Update()
{
	FramebufferTexture shadowMap = FramebufferTexture();
	shadowMap.Create(1024, 1024);

	CreateShadowMap(shadowMap);

	// reset viewport after shadow mapping
	ContextWindow::Instance().ResetViewport();

	FOREACHLINKEDLIST(CCamera*, cam, cameras)
	{
		FOREACHLINKEDLIST(CLight*, light, lights)
		{
			
		}


	}
}

void RenderingSystem::CreateShadowMap(FramebufferTexture &shadowMap)
{	
	//graphics->ClearColor();
	//graphics->Viewport(0, 0, shadowMap.GetWidth(), shadowMap.GetHeight());
	//graphics->BindFramebuffer(shadowMap.GetFBO());

	FOREACHLINKEDLIST(CLight*, light, lights)
	{
		// depth shader for shadowmaps
		// MUST BE INITIALIZED
		Shader depthShader;

		/*for (UINT i = 0; i < lastMeshId; i++)
		{
			Mesh *mesh;
			if (meshes.Find(i, mesh))
			{
				if (mesh->isActive)
				{
					mesh->Draw(depthShader);
				}
			}
		}*/
	}

	// reset framebuffer
	//graphics->BindFramebuffer(0);
}

RenderingSystem * RenderingSystem::Instance()
{
	return instance;
}

void RenderingSystem::Register(Mesh * mesh)
{
	mesh->meshId = lastMeshId;
	meshes.Add(lastMeshId, mesh);
	lastMeshId++;
}

void RenderingSystem::Register(Material * material)
{
	material->materialId = lastMaterialId;
	materials.Add(lastMaterialId, material);
	lastMaterialId++;
}

void RenderingSystem::Register(Texture * texture)
{
	texture->textureId = lastTextureId;
	textures.Add(lastTextureId, texture);
	lastTextureId++;
}

void RenderingSystem::Register(Shader * shader)
{
	shader->shaderId = lastShaderId;
	shaders.Add(lastShaderId, shader);
	lastShaderId++;
}

void RenderingSystem::Register(Mesh &mesh, Material &material)
{
	LinkedList<MeshID> *list = GetMatMesh(material.materialId);
	list->Add(mesh.meshId);
}

Mesh * RenderingSystem::GetMesh(MeshID id) const
{
	Mesh *result;
	meshes.Find(id, result);

	return result;
}

Material * RenderingSystem::GetMaterial(MaterialID id) const
{
	Material *result;
	materials.Find(id, result);

	return result;
}

Texture * RenderingSystem::GetTexture(TextureID id) const
{
	Texture *result;
	textures.Find(id, result);

	return result;
}

Shader * RenderingSystem::GetShader(ShaderID id) const
{
	Shader *result;
	shaders.Find(id, result);

	return result;
}

inline LinkedList<MeshID>* RenderingSystem::GetMatMesh(MaterialID id)
{
	LinkedList<MeshID> *result;
	matMeshes.Find(id, result);

	return result;
}

const LinkedList<MeshID>* RenderingSystem::GetMatMesh(MaterialID id) const
{
	LinkedList<MeshID> *result;
	matMeshes.Find(id, result);

	return result;
}

