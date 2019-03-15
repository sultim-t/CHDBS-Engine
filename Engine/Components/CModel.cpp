#include "CModel.h"
#include <Engine/Math/Vector.h>
#include <Engine/Components/Mesh.h>
#include <Engine/Entities/Entity.h>
#include <Engine/Rendering/Material.h>
#include <Engine/ResourceManager/ResourceManager.h>
#include <Engine/Systems/RenderingSystem.h>

#define PROPERTY_KEY_TYPE			"path"
#define PROPERTY_KEY_CASTSHADOWS	"castShadows"
#define PROPERTY_KEY_RECEIVESHADOWS	"receiveShadows"

CLASSDEFINITION(IComponent, CModel)

CModel::CModel() : IsCastingShadows(true), IsReceivingShadows(true), modelResource(nullptr)
{ }

void CModel::BindPath(const char *path)
{
	this->path = path;
}

const std::vector<Mesh> &CModel::GetMeshes() const
{
	return meshes;
}

void CModel::Init()
{
	RenderingSystem::Instance().Register(this);

	// load model
	modelResource = ResourceManager::Instance().LoadModel(path);

	// init meshes
	auto &meshesSource = modelResource->GetMeshes();
	for (UINT i = 0; i < meshesSource.size(); i++)
	{
		meshes.push_back(Mesh(meshesSource[i]));
	}
}

void CModel::Update()
{ }

void CModel::SetProperty(const String &key, const String &value)
{
	if (key == PROPERTY_KEY_TYPE)
	{
		path = value;
	}
	else if (key == PROPERTY_KEY_CASTSHADOWS)
	{
		IsCastingShadows = value.ToBool();
	}
	else if (key == PROPERTY_KEY_RECEIVESHADOWS)
	{
		IsReceivingShadows = value.ToBool();
	}
	else
	{
		Logger::Print("Wrong model property");
	}
}
