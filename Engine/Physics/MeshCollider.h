#pragma once
#include "ICollider.h"

class MeshCollider : public ICollider
{
private:
	const MeshResource *mesh;

public:
	// Empty constructor
	inline MeshCollider();
	inline MeshCollider(const MeshResource *mesh);

	inline const MeshResource &GetMesh() const;

	inline ColliderType GetColliderType() const override;
	bool Intersect(const ICollider & col) const override;
};

inline MeshCollider::MeshCollider()
{
	this->mesh = nullptr;
}

inline MeshCollider::MeshCollider(const MeshResource * mesh)
{
	this->mesh = mesh;
}

inline const MeshResource & MeshCollider::GetMesh() const
{
	return *mesh;
}

inline ColliderType MeshCollider::GetColliderType() const
{
	return ColliderType::Mesh;
}
