#include "CModel.h"

#include <Engine/Systems/RenderingSystem.h>
#include <Engine/Rendering/OpenGL.h>
#include <Engine/Rendering/Vertex.h>
#include <Engine/ResourceManager/ResourceManager.h>
#include <Engine/ResourceManager/MeshResource.h>

CLASSDEFINITION(IComponent, CModel)

CModel::CModel() : IsCastingShadows(true), IsReceivingShadows(true), modelResource(nullptr), correctionMatrix(1.0f, true)
{ }

const StaticArray<MeshResource*> &CModel::GetMeshes() const
{
	return modelResource->GetHierarchy().GetMeshes();
}

const StaticArray<StandardMaterial*>& CModel::GetMaterials() const
{
	return modelResource->GetMaterials();
}

const StaticArray<UINT>& CModel::GetVAO() const
{
	return vaos;
}

void CModel::Init()
{
	// register to render
	//RenderingSystem::Instance().Register(this);

	// load model
	modelResource = ResourceManager::Instance().LoadModel(path);
	UINT meshesCount = modelResource->GetMeshesCount();

	// init 
	vaos.Init(meshesCount);
	vbos.Init(meshesCount);
	ibos.Init(meshesCount);

	// init for rendering
	InitStatic();

	BindShader(shader);
}

void CModel::Update()
{ }

void CModel::BindShader(const char *shaderName)
{
	const StaticArray<StandardMaterial*> &materials = modelResource->GetMaterials();

	for (UINT i = 0; i < materials.GetSize(); i++)
	{
		materials[i]->BindShader(shaderName);

		// all properties are set
		// init locations
		materials[i]->InitUniformLocations();
	}

	// if there is forced diffuse texture, add it to each material
	if (forcedDiffuseTexture.Length() != 0)
	{
		Texture *texture = new Texture();
		texture->Load(forcedDiffuseTexture);

		for (UINT i = 0; i < materials.GetSize(); i++)
		{
			materials[i]->AddTexture(texture);
		}
	}
}

#define PROPERTY_KEY_TYPE			"Path"
#define PROPERTY_KEY_CASTSHADOWS	"CastShadows"
#define PROPERTY_KEY_RECEIVESHADOWS	"ReceiveShadows"
#define PROPERTY_KEY_CORRECTPOS		"PositionCorrection"
#define PROPERTY_KEY_CORRECTSCALE	"ScaleCorrection"
#define PROPERTY_KEY_CORRECTROT		"RotationCorrection"
#define PROPERTY_KEY_SHADER			"Shader"
#define PROPERTY_KEY_DIFFTEXTURE	"ForcedDiffuseTexture"

void CModel::SetProperty(const String &key, const String &value)
{
	if (key == PROPERTY_KEY_TYPE)
	{
		path = value;
	}
	else if (key == PROPERTY_KEY_CASTSHADOWS)
	{
		IsCastingShadows = value.ToBool();
	}
	else if (key == PROPERTY_KEY_RECEIVESHADOWS)
	{
		IsReceivingShadows = value.ToBool();
	}
	else if (key == PROPERTY_KEY_CORRECTROT)
	{
		Vector3 axis;
		float angle;

		Quaternion q = value.ToQuaternion();
		q.ToAxisAngle(axis, angle);

		correctionMatrix = Transform::RotateMatrix(correctionMatrix, axis, angle);
	}
	else if (key == PROPERTY_KEY_CORRECTSCALE)
	{
		Vector3 scale = value.ToVector3();
		correctionMatrix = Transform::ScaleMatrix(correctionMatrix, scale);
	}
	else if (key == PROPERTY_KEY_CORRECTPOS)
	{
		Vector3 pos = value.ToVector3();
		correctionMatrix = Transform::TranslateMatrix(correctionMatrix, pos);
	}
	else if (key == PROPERTY_KEY_SHADER)
	{
		shader = value;
	}
	else if (key == PROPERTY_KEY_DIFFTEXTURE)
	{
		forcedDiffuseTexture = value;
	}
	else
	{
		Logger::Print("Wrong model property");
	}
}

void CModel::InitMesh(const MeshResource *resource, UINT &vao, UINT &vbo, UINT &ibo, bool useTan, bool dynamic)
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	glBindVertexArray(vao);
	// load to vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	UINT size = useTan ? sizeof(Vertex5) : sizeof(Vertex);

	if (!dynamic)
	{
		glBufferData(GL_ARRAY_BUFFER, resource->GetVertices().GetSize() * size, resource->GetVertices().GetArray(), GL_STATIC_DRAW);
	}
	else
	{
		glBufferData(GL_ARRAY_BUFFER, resource->GetVertices().GetSize() * size, nullptr, GL_STREAM_DRAW);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	// indices must not be changed
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, resource->GetIndices().GetSize() * sizeof(unsigned int), resource->GetIndices().GetArray(), GL_STATIC_DRAW);

	// set attributes
	// positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, size, (void*)0);
	// normals
	glEnableVertexAttribArray(1);
	if (useTan)
	{
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, size, (void*)offsetof(Vertex5, Normal));
	}
	else
	{
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, size, (void*)offsetof(Vertex, Normal));
	}

	// texture coords
	glEnableVertexAttribArray(2);
	if (useTan)
	{
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, size, (void*)offsetof(Vertex5, TexCoords));
	}
	else
	{
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, size, (void*)offsetof(Vertex, TexCoords));
	}

	if (useTan)
	{
		// tangent
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, size, (void*)offsetof(Vertex5, Tangent));
		// bitangent
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, size, (void*)offsetof(Vertex5, Bitangent));
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

inline void CModel::InitStatic()
{
	UINT meshesCount = modelResource->GetMeshesCount();
	for (UINT i = 0; i < meshesCount; i++)
	{
		// init for rendering
		InitMesh(modelResource->GetHierarchy().GetMeshes()[i], vaos[i], vbos[i], ibos[i], true, false);
	}
}