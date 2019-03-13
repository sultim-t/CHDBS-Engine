#pragma once

#include "IComponent.h"
#include "CModel.h"
#include <Engine/Components/CMesh.h>
#include <Engine/ResourceManager/ModelResource.h>

class CSkinnedModel : public CModel
{
	friend class ResourceManager;
	
private:


public:
	void Init() override;
	void Update() override;
};