#include "CModel.h"
#include <Engine/Math/Vector.h>
#include <Engine/Rendering/Texture.h>
#include <Engine/Components/CMesh.h>
#include <Engine/Entities/Entity.h>
#include <Engine/Rendering/Material.h>
#include <Engine/ResourceManager/ResourceManager.h>
#include <Engine/Systems/RenderingSystem.h>
#include <Engine/Base/String.h>

#define PROPERTY_KEY_TYPE "path"

CLASSDEFINITION(IComponent, CModel)

void CModel::BindPath(const char * path)
{
	this->path = path;
}

void CModel::Draw()
{
	if (!IsActive())
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

	// load model
	ResourceManager::LoadModel(path, *this);
}

void CModel::Update()
{ }

void CModel::SetProperty(const String & key, const String & value)
{
	if (key == PROPERTY_KEY_TYPE)
	{
		path = value;
	}
	else
	{
		Logger::Print("Wrong model property");
	}
}
