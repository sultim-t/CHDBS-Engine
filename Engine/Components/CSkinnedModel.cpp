#include "CSkinnedModel.h"

#include <Engine/Math/Vector.h>
#include <Engine/Rendering/Animation.h>
#include <Engine/Rendering/AnimationNode.h>
#include <Engine/Rendering/Bone.h>
#include <Engine/Rendering/Skeleton.h>
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
		const StaticArray<Vertex5> &sourceVerts = meshes[i]->GetVertices();
		UINT vertsCount = sourceVerts.GetSize();

		// create vertex array for each mesh
		vertices[i] = new StaticArray<Vertex5>();
		// init with same size
		vertices[i]->Init(vertsCount);

		// copy data
		for (UINT v = 0; v < vertsCount; v++)
		{
			vertices[i]->operator[](v) = sourceVerts[v];
		}
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

	float animLength = animation->GetDuration() / animation->GetTicksPerSecond();

	if (currentTime > animLength)
	{
		currentTime = Mod(currentTime, animLength);
	}

	float timeInTicks = currentTime * animation->GetTicksPerSecond();
	float animTime = Mod(timeInTicks, animation->GetDuration());

	const StaticArray<AnimationNode*> &nodes = animation->GetAnimationNodes();


	int meshIndex = 0;
	MeshResource *mesh = modelResource->GetHierarchy().GetMeshes()[meshIndex];
	const StaticArray<Vertex5> &sourceVerts = mesh->GetVertices();


	StaticArray<Vertex5> &newVerts = *vertices[meshIndex];

	const Skeleton &skeleton = mesh->GetSkeleton();


	// update matrices for bones
	skeleton.UpdateBoneMatrices(animation, currentTime);

	// load vertices to this buffer
	skeleton.UpdateVertices(newVerts);

	// load model to gpu
	GFXUpdate(meshIndex);


	// update time for animation
	currentTime += Time::GetDeltaTime();
}

void CSkinnedModel::SetProperty(const String & key, const String & value)
{
	CModel::SetProperty(key, value);
}
