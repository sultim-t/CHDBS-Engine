#include "CModel.h"
#include <Engine/Math/Vector.h>
#include <Engine/Components/Mesh.h>
#include <Engine/Entities/Entity.h>
#include <Engine/Rendering/Material.h>
#include <Engine/ResourceManager/ResourceManager.h>
#include <Engine/Systems/RenderingSystem.h>

#define PROPERTY_KEY_TYPE			"Path"
#define PROPERTY_KEY_CASTSHADOWS	"CastShadows"
#define PROPERTY_KEY_RECEIVESHADOWS	"ReceiveShadows"

CLASSDEFINITION(IComponent, CModel)

CModel::CModel() : IsCastingShadows(true), IsReceivingShadows(true), modelResource(nullptr)
{ }

const StaticArray<MeshResource*> &CModel::GetMeshes() const
{
	return modelResource->GetHierarchy().GetMeshes();
}

const StaticArray<Material*>& CModel::GetMaterials() const
{
	return modelResource->GetMaterials();
}

const StaticArray<UINT>& CModel::GetVAO() const
{
	return modelResource->GetVAO();
}

void CModel::Init()
{
	RenderingSystem::Instance().Register(this);

	// load model
	modelResource = ResourceManager::Instance().LoadModel(path);
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
