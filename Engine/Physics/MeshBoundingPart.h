#pragma once
#include <Engine/DataStructures/DynamicArray.h>
#include <Engine/Math/AABB.h>
#include <Engine/Math/Triangle.h>

class MeshBoundingPart
{
private:
	AABB boundingBox;
	Sphere boundingSphere;

	DynamicArray<int> trianglesIndices;

public:
	// Empty constructor
	MeshBoundingPart();
	// Empty destructor (array will be deleted automatically)
	~MeshBoundingPart();

	// Allocate memory
	void Init(const Vector3 &min, const Vector3 &max);

	// Try to add triangle to this part
	bool TryToAdd(const Triangle &tr, int index);
	// Does bounding box contain triangle?
	bool Contains(const Triangle &tr);

	const AABB &GetBoundingBox() const;
	const Sphere &GetBoundingSphere() const;
	const DynamicArray<int> &GetTrianglesIndices() const;

	// Deallocate memory
	void Delete();
};

inline MeshBoundingPart::MeshBoundingPart()
{ }

inline MeshBoundingPart::~MeshBoundingPart()
{ }

inline void MeshBoundingPart::Init(const Vector3 &min, const Vector3 &max)
{
	// init box
	boundingBox.SetMin(min);
	boundingBox.SetMax(max);

	// init sphere
	float radius = boundingBox.GetExtent().Length();
	boundingSphere.SetCenter(boundingBox.GetCenter());
	boundingSphere.SetRadius(radius);

	// suppose, that mesh bounding part will be used as value type (not reference type)
	trianglesIndices.RawInit(16);
}

inline bool MeshBoundingPart::TryToAdd(const Triangle & tr, int index)
{
	// if at least one is in bounding box
	if (boundingBox.Contains(tr.A) || boundingBox.Contains(tr.B) || boundingBox.Contains(tr.C))
	{
		// add triangle's index
		trianglesIndices.Push(index);

		return true;
	}

	return false;
}

inline bool MeshBoundingPart::Contains(const Triangle & tr)
{
	return boundingBox.Contains(tr.A) || boundingBox.Contains(tr.B) || boundingBox.Contains(tr.C);
}

inline const AABB &MeshBoundingPart::GetBoundingBox() const
{
	return boundingBox;
}

inline const Sphere &MeshBoundingPart::GetBoundingSphere() const
{
	return boundingSphere;
}

inline const DynamicArray<int> &MeshBoundingPart::GetTrianglesIndices() const
{
	return trianglesIndices;
}

inline void MeshBoundingPart::Delete()
{
	// delete array
	trianglesIndices.Delete();
}
