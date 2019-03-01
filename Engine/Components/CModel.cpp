#include "CModel.h"
#include <Engine/Math/Vector.h>
#include <Engine/Rendering/Texture.h>
#include <Engine/Components/CMesh.h>
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

void CModel::BindPath(const char * path)
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
float t = 0;

void CModel::Update()
{ 
	// todo: delete
	
	Vector3 poses[] = { Vector3(0.0f), Vector3(0,0,1), Vector3(0.0f) };
	Vector3 eulers[] = { Vector3(0, 0, 0), Vector3(0, 30, 0), Vector3(0, 0, 0) };
	float times[] = { 0, 5.0f / 60.0f, 60.0f / 60.0f };

	t += Time::GetDeltaTime();
	if (meshes[0].GetName() == "2barrel")
	{
		for (int k = 0; k < 3 - 1; k++)
		{
			if (t >= times[k] && t < times[k+1])
			{
				float time = t - times[k];
				time /= times[k + 1] - times[k];

				meshes[0].GetTransform().SetPosition(Vector3::Lerp(poses[k], poses[k + 1], time));
				meshes[0].GetTransform().SetRotation(Vector3::Lerp(eulers[k], eulers[k + 1], time));

				break;
			}
		}

		if (t > times[3 - 1])
		{
			t = 0.0f;
		}
	}
}

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
