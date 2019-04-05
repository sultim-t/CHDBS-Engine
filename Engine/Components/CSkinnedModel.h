#pragma once

#include "IComponent.h"
#include "CModel.h"
#include <Engine/ResourceManager/ModelResource.h>

class CSkinnedModel : public CModel
{
	friend class ResourceManager;
	
	CLASSDECLARATION(CModel)

private:
	// temporary vertex buffer, for animations
	// for each mesh
	StaticArray<StaticArray<Vertex5>*>	vertices;

	int currentAnimation;
	float currentTime;

private:
	// Init as dynamic (deformable) model
	void InitDynamic();

public:
	~CSkinnedModel();

	void Init() override;
	void Update() override;
	void SetProperty(const String &key, const String &value) override;
};