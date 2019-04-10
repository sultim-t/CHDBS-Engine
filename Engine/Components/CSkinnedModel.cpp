#include "CSkinnedModel.h"

#include <Engine/Math/Vector.h>
#include <Engine/Rendering/Animation.h>
#include <Engine/Rendering/AnimationNode.h>
#include <Engine/Rendering/Bone.h>
#include <Engine/Rendering/OpenGL.h>
#include <Engine/Systems/RenderingSystem.h>
#include <Engine/ResourceManager/ResourceManager.h>
#include <Engine/ResourceManager/MeshResource.h>

CLASSDEFINITION(CModel, CSkinnedModel)

void CSkinnedModel::InitDynamic()
{
	UINT meshesCount = modelResource->GetMeshesCount();
	for (UINT i = 0; i < meshesCount; i++)
	{
		// init for rendering
		InitMesh(modelResource->GetHierarchy().GetMeshes()[i], vaos[i], vbos[i], ibos[i], true, true); // <- only difference
	}
}

void CSkinnedModel::GFXUpdate(int meshIndex)
{
	// bind current mesh
	glBindVertexArray(vaos[meshIndex]);
	// bind mesh's buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbos[meshIndex]);

	// update data in buffer
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices[meshIndex]->GetSize() * sizeof(Vertex5), vertices[meshIndex]->GetArray());

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void CSkinnedModel::Init()
{
	// register to render
	RenderingSystem::Instance().Register(this);

	// load model
	modelResource = ResourceManager::Instance().LoadModel(path);
	
	// get meshes
	const StaticArray<MeshResource*> &meshes = modelResource->GetHierarchy().GetMeshes();
	UINT meshesCount = modelResource->GetMeshesCount();

	// init 
	vaos.Init(meshesCount);
	vbos.Init(meshesCount);
	ibos.Init(meshesCount);

	// init vertices
	vertices.Init(meshesCount);
	for (UINT i = 0; i < meshesCount; i++)
	{
		vertices[i] = new StaticArray<Vertex5>();

		// init for each mesh
		vertices[i]->Init(meshes[i]->GetVertices().GetSize());
	}

	// init for rendering
	InitDynamic();
}

CSkinnedModel::~CSkinnedModel()
{
	UINT meshesCount = modelResource->GetMeshesCount();
	for (UINT i = 0; i < meshesCount; i++)
	{
		delete vertices[i];
	}
}

void CSkinnedModel::Update()
{
	const Animation *animation = modelResource->GetHierarchy().GetAnimations()[currentAnimation];

	if (currentTime > animation->GetDuration())
	{
		currentTime -= animation->GetDuration();
	}

	const StaticArray<AnimationNode*> &nodes = animation->GetAnimationNodes();

	int meshIndex = 0;

	MeshResource *mesh = modelResource->GetHierarchy().GetMeshes()[meshIndex];
	StaticArray<Vertex5> &meshVertices = *vertices[meshIndex];

	// for each vertex
	for (UINT v = 0; v < meshVertices.GetSize(); v++)
	{
		meshVertices[v] = mesh->GetVertices()[v];
	}

	// for each animation node
	//for (UINT i = 0; i < nodes.GetSize(); i++)
	{
		const StaticArray<Bone*> &bones = mesh->GetBones();

		// for each bone
		for (UINT i = 0; i < bones.GetSize(); i++)
		{
			Matrix4 boneTransform = bones[i]->GetTranformationMatrix();

			for (UINT j = 0; j < bones[i]->GetWeightCount(); j++)
			{
				const VertexWeight &w = bones[i]->GetWeight(j);

				// get affected vertex
				const Vertex5 &vertex = mesh->GetVertices()[w.VertexID];

				Matrix4 boneTransformWeighted = boneTransform * w.Weight;
				meshVertices[w.VertexID].Position = boneTransformWeighted * vertex.Position;
			}
		}

		//nodes[i]->GetInterpolatedPosition(currentTime, );
	}

	GFXUpdate(meshIndex);

	currentTime += Time::GetDeltaTime();
}

void CSkinnedModel::SetProperty(const String & key, const String & value)
{
	CModel::SetProperty(key, value);
}
