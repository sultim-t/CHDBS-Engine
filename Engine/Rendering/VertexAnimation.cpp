#include "VertexAnimation.h"
#include <Engine/ResourceManager/ResourceManager.h>
#include <Engine/ResourceManager/VertexAnimatedResource.h>

int VertexAnimation::GetModelIndex(float time) const
{
	auto &keys = vertAnim->GetAnimationNodes();
	UINT count = keys.GetSize();

	for (UINT i = 0; i < count - 1; i++)
	{
		if (keys[i].Time <= time && time < keys[i + 1].Time)
		{
			return (int)i;
		}
	}

	return 0;
}

VertexAnimation::VertexAnimation() : WrapType(AnimationWrapType::Once)
{ }

void VertexAnimation::Init(const char * path)
{
	// load from resource
	vertAnim = ResourceManager::Instance().LoadVertexAnimated(path);
	animLength = vertAnim->GetDuration() / vertAnim->GetTicksPerSecond();
}

bool VertexAnimation::Animate(StaticArray<StaticArray<Vertex5>*>& tempVerts, float time) const
{
	const ModelResource *prevModel, *nextModel;
	float t;

	bool animated = GetModels(time, prevModel, nextModel, t);

	const StaticArray<MeshResource*> &prevMeshes = prevModel->GetHierarchy().GetMeshes();
	const StaticArray<MeshResource*> &nextMeshes = nextModel->GetHierarchy().GetMeshes();

	UINT meshesCount = tempVerts.GetSize();
	ASSERT(meshesCount = prevMeshes.GetSize());
	ASSERT(meshesCount = nextMeshes.GetSize());

	// update each mesh
	for (UINT meshIndex = 0; meshIndex < meshesCount; meshIndex++)
	{
		StaticArray<Vertex5> &targetVerts = *tempVerts[meshIndex];

		// get previous mesh
		const StaticArray<Vertex5> &prevVerts = prevMeshes[meshIndex]->GetVertices();

		// get next mesh
		const StaticArray<Vertex5> &nextVerts = nextMeshes[meshIndex]->GetVertices();

		// interpolate between prev and next
		for (UINT i = 0; i < prevVerts.GetSize(); i++)
		{
			targetVerts[i].Position = Vector3::Lerp(prevVerts[i].Position, nextVerts[i].Position, t);
			targetVerts[i].Normal = Vector3::Lerp(prevVerts[i].Normal, nextVerts[i].Normal, t);
		}
	}
	
	return animated;
}

bool VertexAnimation::GetModels(float time, const ModelResource *& prevModel, const ModelResource *& nextModel, float &interpFactor) const
{
	using namespace Mathf;

	auto &keys = vertAnim->GetAnimationNodes();

	if (WrapType == AnimationWrapType::Loop)
	{
		time = Mod(time, animLength);
	}
	else if (WrapType == AnimationWrapType::Once)
	{
		if (time > animLength)
		{
			// use last frame
			prevModel = keys[keys.GetSize() - 1].Value;
			nextModel = keys[keys.GetSize() - 1].Value;
			interpFactor = 0;

			return false;
		}
	}

	// get model index in array
	int modelId = GetModelIndex(time);
	int nextModelId = modelId + 1;

	// clamp
	if ((UINT)nextModelId >= keys.GetSize())
	{
		nextModelId = 0;
	}

	prevModel = keys[modelId].Value;
	nextModel = keys[nextModelId].Value;
	interpFactor = (time - keys[modelId].Time) / (keys[nextModelId].Time - keys[modelId].Time);

	return true;
}

const VertexAnimatedResource * VertexAnimation::GetVertexAnimationResource() const
{
	return vertAnim;
}

const String & VertexAnimation::GetName() const
{
	return vertAnim->GetName();
}

float VertexAnimation::GetLength() const
{
	return animLength;
}