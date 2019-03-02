#pragma once
#include "ICollider.h"
#include <Engine/ResourceManager/MeshColliderResource.h>
#include <Engine/DataStructures/StaticArray.h>

class MeshCollider : public ICollider
{
private:
	const StaticArray<Triangle> *triangles;

public:
	// Empty constructor
	inline MeshCollider();
	inline MeshCollider(const MeshColliderResource *mesh);
	inline MeshCollider(const StaticArray<Triangle> *tr);

	inline const MeshColliderResource &GetMesh() const;
	inline const StaticArray<Triangle> &GetTriangles() const;

	inline ColliderType GetColliderType() const override;
	bool Intersect(const ICollider &col, CollisionInfo &info) const override;
};

inline MeshCollider::MeshCollider()
{
	this->triangles = nullptr;
}

inline MeshCollider::MeshCollider(const MeshColliderResource *mesh)
{
	this->triangles = &mesh->GetTriangles();
}

inline MeshCollider::MeshCollider(const StaticArray<Triangle>* tr)
{
	triangles = tr;
}

inline const StaticArray<Triangle>& MeshCollider::GetTriangles() const
{
	return *triangles;
}

inline ColliderType MeshCollider::GetColliderType() const
{
	return ColliderType::Mesh;
}
