#include "CSkinnedModel.h"

#include <Engine/Math/Vector.h>
#include <Engine/Rendering/Animation.h>
#include <Engine/Rendering/AnimationNode.h>
#include <Engine/Rendering/Bone.h>
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
	/*const Animation *animation = modelResource->GetHierarchy().GetAnimations()[currentAnimation];

	if (currentTime > animation->GetDuration())
	{
		currentTime -= animation->GetDuration();
	}

	const StaticArray<AnimationNode*> &nodes = animation->GetAnimationNodes();

	int meshIndex = 0;

	MeshResource *mesh = modelResource->GetHierarchy().GetMeshes()[meshIndex];
	auto &arr = *vertices[meshIndex];
	
	for (UINT v = 0; v < arr.GetSize(); v++)
	{
		for (UINT i = 0; i < nodes.GetSize(); i++)
		{
			auto &bones = mesh->GetBones();

			StaticArray<Matrix4> animTransforms;
			animTransforms.Init(bones.GetSize());

			for (UINT i = 0; i < bones.GetSize(); i++)
			{
				for (UINT j = 0; j < bones[j].GetWeightCount(); j++)
				{
					const VertexWeight &w = bones[i].GetWeight(j);

					if (w.VertexID == v)
					{
						arr[v].Position = bones[i].GetMatrix() * w.Weight * Vector4();
					}
				}
			}

			//nodes[i]->GetInterpolatedPosition(currentTime, );
		}
	}

	currentTime += Time::GetDeltaTime();*/
}

void CSkinnedModel::SetProperty(const String & key, const String & value)
{
	CModel::SetProperty(key, value);
}
