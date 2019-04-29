#pragma once

#include "IComponent.h"
#include "CModel.h"
#include <Engine/ResourceManager/ModelResource.h>
#include <Engine/ResourceManager/VertexAnimatedResource.h>

class CVertexAnimated : public CModel
{
	friend class ResourceManager;
	
	CLASSDECLARATION(CModel)

private:
	// Contains time values and path to models
	const VertexAnimatedResource *vertAnim;
	// Temp vertices for animation
	// For each mesh, for each vertex
	StaticArray<StaticArray<Vertex5>*> tempVerts;

	StaticArray<const ModelResource*> models;
	StaticArray<float> modelsTime;

	String vertAnimPath;
	float currentTime;

private:
	// Init as dynamic (deformable) model
	void InitDynamic();
	// Update graphics arrays
	void GFXUpdate();

	int GetModelID(float time);

public:
	~CVertexAnimated();

	void Init() override;
	void Update() override;
	void SetProperty(const String &key, const String &value) override;
};