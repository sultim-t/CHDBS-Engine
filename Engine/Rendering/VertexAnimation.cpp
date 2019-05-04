#include "VertexAnimation.h"
#include <Engine/ResourceManager/ResourceManager.h>
#include <Engine/ResourceManager/VertexAnimatedResource.h>

int VertexAnimation::GetModelIndex(float time)
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

void VertexAnimation::Init(const char * path)
{
	// load from resource
	vertAnim = ResourceManager::Instance().LoadVertexAnimated(path);
	animLength = vertAnim->GetDuration() / vertAnim->GetTicksPerSecond();
}

void VertexAnimation::Animate(StaticArray<StaticArray<Vertex5>*>& tempVerts, float time)
{
	// get model index in array
	int modelId = GetModelIndex(time);
	int nextModelId = modelId + 1;

	auto &keys = vertAnim->GetAnimationNodes();

	// clamp
	if ((UINT)nextModelId >= keys.GetSize())
	{
		nextModelId = 0;
	}

	UINT meshesCount = tempVerts.GetSize();
	const StaticArray<MeshResource*> &prevMeshes = keys[modelId].Value->GetHierarchy().GetMeshes();
	const StaticArray<MeshResource*> &nextMeshes = keys[nextModelId].Value->GetHierarchy().GetMeshes();

	ASSERT(meshesCount = prevMeshes.GetSize());
	ASSERT(meshesCount = nextMeshes.GetSize());

	// get interpolation factor
	float t = (time - keys[modelId].Time) / (keys[nextModelId].Time - keys[modelId].Time);

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
