#pragma once
#include <vector>

class ModelResource
{
private:
	typedef std::vector<const MeshResource*> MeshesContainer;

	MeshesContainer meshes;
	String path;

public:
	inline ModelResource(const char *path);
	inline ModelResource(const char *path, const MeshesContainer &meshes);

	inline void AddMesh(const MeshResource *mesh);

	inline const MeshesContainer &GetMeshes() const;
	inline const String &GetPath() const;
};

inline ModelResource::ModelResource(const char * path)
{
	this->path = path;
}

inline ModelResource::ModelResource(const char * path, const MeshesContainer & meshes)
{
	this->path = path;
	this->meshes = meshes;
}

inline void ModelResource::AddMesh(const MeshResource * mesh)
{
	meshes.push_back(mesh);
}

inline const ModelResource::MeshesContainer & ModelResource::GetMeshes() const
{
	return meshes;
}

inline const String & ModelResource::GetPath() const
{
	return path;
}