#include "MeshCollider.h"
#include "AABBCollider.h"
#include "SphereCollider.h"
#include <Engine/Rendering/DebugDrawer.h>
#include <Engine/Math/Intersection.h>

const StaticArray<Triangle> &MeshCollider::GetTriangles() const
{
	return triangles;
}

ColliderType MeshCollider::GetColliderType() const
{
	return ColliderType::Mesh;
}

Sphere MeshCollider::GetBoundingSphere() const
{
	return boundingSphere;
}

PhysicMaterial MeshCollider::GetPhysicMaterial() const
{
	return physicMaterial;
}

bool MeshCollider::Intersect(const ICollider & col, CollisionInfo &info) const
{
	// this function must be called only from SphereCollider/AABBCollider

	// first, test with bounding spheres/boxes of parts
	// then if there is an intersection, try to test triangles of current part

	switch (col.GetColliderType())
	{
	case ColliderType::AABB:
	{
		const AABB &other = ((AABBCollider&)col).GetAABB();
		
		bool atLeastOne = false;

		for (UINT i = 0; i < partsCount; i++)
		{
			// if there is no intersection with part, move to next
			if (!Intersection::AABBAABB(parts[i].GetBoundingBox(), other))
			{
				continue;
			}

			// If intersected with part's bounding,
			// test triangles using part's indices
			// Get all contacts
			if (Intersection::MeshAABB(triangles, parts[i].GetTrianglesIndices(), other, info))
			{
				atLeastOne = true;

				// if array in info is full
				if (!info.HasFree())
				{
					return true;
				}

				// else continue checking
				continue;
			}
		}

		return atLeastOne;
	}
	case ColliderType::Sphere:
	{	
		const Sphere &other = ((SphereCollider&)col).GetSphere();
		
		bool atLeastOne = false;

		for (UINT i = 0; i < partsCount; i++)
		{
			// if there is no intersection with part, move to next
			if (!Intersection::SphereSphere(parts[i].GetBoundingSphere(), other))
			{
				continue;
			}
		
			DebugDrawer::Instance().Draw(parts[i].GetBoundingBox());

			// If intersected with part's bounding,
			// test triangles using part's indices
			// Get all contacts
			if (Intersection::MeshSphere(triangles, parts[i].GetTrianglesIndices(), other, info))
			{
				atLeastOne = true;

				// if array in info is full
				if (!info.HasFree())
				{
					return true;
				}

				// else continue checking other blocks
				continue;
			}
		}

		return atLeastOne;
	}
	default:
		// other intersections are not implemeted
		ASSERT(0);
	}

	return false;
}

void MeshCollider::RecalculateBoundingBox()
{
	UINT size = triangles.GetSize();

	Vector3 min = Vector3(0.0f);
	Vector3 max = Vector3(0.0f);

	// for each triangle
	for (UINT t = 0; t < size; t++)
	{
		const Triangle &triangle = triangles[t];

		// for each vertex in triangle
		for (int i = 0; i < 3; i++)
		{
			const Vector3 &vertex = triangle[i];

			// for each dimension in vector
			for (int j = 0; j < 3; j++)
			{
				float d = vertex[j];

				if (d < min[j])
				{
					min[j] = d;
				}

				if (d > max[j])
				{
					max[j] = d;
				}
			}
		}
	}

	// set
	boundingBox.SetMax(max);
	boundingBox.SetMin(min);
}

void MeshCollider::RecalculateBoundingSphere()
{
	float radius = boundingBox.GetExtent().Length();
	boundingSphere = Sphere(boundingBox.GetCenter(), radius);
}


void MeshCollider::AddTriangles(const StaticArray<Triangle> &mesh)
{
	AddTriangles(mesh, Transform());
}

void MeshCollider::AddTriangles(const StaticArray<Triangle> &mesh, const Transform &t)
{
	UINT curSize = triangles.GetSize();
	UINT addingSize = mesh.GetSize();
	UINT newSize = curSize + addingSize;

	// save old data
	StaticArray<Triangle> &oldData = triangles.GetCopy();

	// delete old array
	triangles.Delete();
	// reinit with new size
	triangles.Init(newSize);

	// copy old data to new array
	for (UINT i = 0; i < curSize; i++)
	{
		triangles[i].A = oldData[i].A;
		triangles[i].B = oldData[i].B;
		triangles[i].C = oldData[i].C;
	}

	// for each new triangle: add it transformed
	for (UINT i = curSize; i < newSize; i++)
	{
		const Triangle &cur = mesh[i - curSize];

		// transform
		triangles[i].A = t.PointFromLocal(cur.A);
		triangles[i].B = t.PointFromLocal(cur.B);
		triangles[i].C = t.PointFromLocal(cur.C);
	}
}

void MeshCollider::RecalculateBoundingParts()
{
	// all triangles are set and have new indices, so delete old data
	// (if just allocated then nothing will be changed)
	DeleteParts();

	const Vector3 &min = boundingBox.GetMin();
	const Vector3 &max = boundingBox.GetMax();

	// step
	float delta = MESH_COLLIDER_PART_SIZE;

	// get discrete min and max
	int minX = (int)(min[0] / delta) - 1;
	int minY = (int)(min[1] / delta) - 1;
	int minZ = (int)(min[2] / delta) - 1;

	int maxX = (int)(max[0] / delta) + 1;
	int maxY = (int)(max[1] / delta) + 1;
	int maxZ = (int)(max[2] / delta) + 1;

	int partsXCount = maxX - minX;
	int partsYCount = maxY - minY;
	int partsZCount = maxZ - minZ;

	partsXCount = partsXCount >= 0 ? partsXCount : -partsXCount;
	partsYCount = partsYCount >= 0 ? partsYCount : -partsYCount;
	partsZCount = partsZCount >= 0 ? partsZCount : -partsZCount;

	// reinit array
	parts.Init(partsXCount * partsYCount * partsZCount);

	Vector3 pmin, pmax;
	UINT partIndex = 0;

	// for each dimension create bounding box with 
	for (int i = 0; i < partsXCount; i++)
	{
		for (int j = 0; j < partsYCount; j++)
		{
			for (int k = 0; k < partsZCount; k++)
			{
				pmin[0] = (minX + i) * delta;
				pmin[1] = (minY + j) * delta;
				pmin[2] = (minZ + k) * delta;

				pmax[0] = (minX + i + 1) * delta;
				pmax[1] = (minY + j + 1) * delta;
				pmax[2] = (minZ + k + 1) * delta;

				// allocate memory and calculate bounding shapes
				parts[partIndex].Init(pmin, pmax);
				partIndex++;
			}
		}
	}

	partsCount = partIndex;

	// for each triangle
	UINT triangleCount = triangles.GetSize();
	for (UINT t = 0; t < triangleCount; t++)
	{		
		// find max and min indices of parts for this triangle
		VectorI3 minIndices = VectorI3(maxX - minX, maxY - minY, maxZ - minZ);
		VectorI3 maxIndices = VectorI3(0, 0, 0);

		// for each triangle corner point
		for (int p = 0; p < 3; p++)
		{
			const Vector3 &point = triangles[t].ABC[p];

			// clamp to get indices
			float fi = point[0] / delta;
			float fj = point[1] / delta;
			float fk = point[2] / delta;

			// get indices of part where this point is contained
			int i = fi >= 0.0f ? (int)(fi) : (int)(fi) - 1;
			int j = fj >= 0.0f ? (int)(fj) : (int)(fj) - 1;
			int k = fk >= 0.0f ? (int)(fk) : (int)(fk) - 1;

			// shift to relative space
			VectorI3 r = VectorI3(i - minX, j - minY, k - minZ);

			for (int l = 0; l < 3; l++)
			{
				minIndices[l] = r[l] < minIndices[l] ? r[l] : minIndices[l];
				maxIndices[l] = r[l] > maxIndices[l] ? r[l] : maxIndices[l];
			}
		}

		// for each part from min indices to max ones
		for (int i = minIndices[0]; i <= maxIndices[0]; i++)
		{
			for (int j = minIndices[1]; j <= maxIndices[1]; j++)
			{
				for (int k = minIndices[2]; k <= maxIndices[2]; k++)
				{
					// get part of this point
					MeshBoundingPart &part = parts[i * partsYCount * partsZCount + j * partsZCount + k];

					// try to add
					part.TryToAdd(triangles[t], t);
				}
			}
		}
	}
}

void MeshCollider::DeleteParts()
{
	// delete all parts
	for (UINT i = 0; i < partsCount; i++)
	{
		parts[i].Delete();
	}

	// delete array itself
	parts.Delete();
}

MeshCollider::~MeshCollider()
{
	DeleteParts();
}