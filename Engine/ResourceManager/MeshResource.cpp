#include "MeshResource.h"
#include <Engine/Rendering/Skeleton.h>

MeshResource::MeshResource(int verticesSize, int indicesSize, int trianglesSize, int bonesSize, UINT materialIndex) 
	: boundingCalculated(false), materialIndex(materialIndex)
{
	ASSERT(verticesSize >= 0 && indicesSize >= 0 && trianglesSize >= 0 && bonesSize >= 0);

	// allocate memory for arrays
	vertices.Init(verticesSize);
	indices.Init(indicesSize);
	triangles.Init(trianglesSize);

	if (bonesSize > 0)
	{
		// setup skeleton with this mesh
		skeleton = new Skeleton(*this, bonesSize, verticesSize);
	}
	else
	{
		// else do not allocate memory
		skeleton = nullptr;
	}
}

MeshResource::~MeshResource()
{
	Delete();
}

const StaticArray<Vertex5> &MeshResource::GetVertices() const
{
	return vertices;
}

const StaticArray<UINT> &MeshResource::GetIndices() const
{
	return indices;
}

const StaticArray<Triangle> &MeshResource::GetTriangles() const
{
	return triangles;
}

const Skeleton &MeshResource::GetSkeleton() const
{
	return *skeleton;
}

UINT MeshResource::GetMaterialIndex() const
{
	return materialIndex;
}

bool MeshResource::HasBones() const
{
	return skeleton != nullptr;
}

/*Bone const *MeshResource::GetBone(const char * name) const
{
UINT count = bones.GetSize();
for (UINT i = 0; i < count; i++)
{
if (bones[i]->GetName() == name)
{
return bones[i];
}
}

return nullptr;
}*/

const Sphere &MeshResource::GetBoundingSphere()
{
	if (boundingCalculated)
	{
		return boundingSphere;
	}

	Vector3 min = Vector3(0.0f);
	Vector3 max = Vector3(0.0f);

	UINT size = vertices.GetSize();
	for (UINT i = 0; i < size; i++)
	{
		for (int k = 0; k < 3; k++)
		{
			float t = vertices[i].Position[k];

			if (t < min[k])
			{
				min[k] = t;
			}
			else if (t > max[k])
			{
				max[k] = t;
			}
		}
	}

	Vector3 center = (max + min) * 0.5f;
	float radius = (max - min).Length() * 0.5f;

	boundingSphere = Sphere(center, radius);
	boundingCalculated = true;

	return boundingSphere;
}

void MeshResource::Delete()
{
	vertices.Delete();
	indices.Delete();
	triangles.Delete();

	delete skeleton;
}
