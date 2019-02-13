#include "CModel.h"
#include <Engine/Math/Vector.h>
#include <Engine/Rendering/Texture.h>
#include <Engine/Components/CMesh.h>
#include <Engine/Entities/Entity.h>
#include <Engine/Rendering/Material.h>
#include <Engine/ResourceManager/ResourceManager.h>
#include <Engine/Systems/RenderingSystem.h>


CLASSDEFINITION(IComponent, CModel)

void CModel::Draw()
{
	if (!isActive)
	{
		return;
	}

	for (unsigned i = 0; i < meshes.size(); i++)
	{
		meshes[i].GetMaterial()
			.BindModelMatrix(owner->GetTransform()
				.GetTransformMatrix());
		meshes[i].Draw();
	}
}

const std::vector<Mesh>& CModel::GetMeshes() const
{
	return meshes;
}

void CModel::Init()
{
	RenderingSystem::Instance().Register(this);
}

void CModel::Update()
{ }

void CModel::Load(const char * path)
{
	ResourceManager::LoadModel(path, *this);
}