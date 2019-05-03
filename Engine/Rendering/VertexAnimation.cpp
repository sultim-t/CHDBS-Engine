#include "VertexAnimation.h"

int VertexAnimation::GetModelIndex(float time)
{
	for (UINT i = 0; i < keys.GetSize() - 1; i++)
	{
		if (keys[i].Time <= time && time < keys[i + 1].Time)
		{
			return (int)i;
		}
	}

	return 0;
}

VertexAnimation::VertexAnimation(const char * name, float duration, float ticksPerSecond)
	: name(name), duration(duration), ticksPerSecond(ticksPerSecond) { }

VertexAnimation::~VertexAnimation()
{ }

void VertexAnimation::Init(int keyCount)
{
	keys.Init(keyCount);
}

void VertexAnimation::Animate(StaticArray<StaticArray<Vertex5>*>& tempVerts, float time)
{
	// get model index in array
	int modelId = GetModelIndex(time);
	int nextModelId = modelId + 1;

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
