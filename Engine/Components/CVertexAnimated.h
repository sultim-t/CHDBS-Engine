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
	// Path to anim file
	String vertAnimPath;
	// All loaded models for interpolating
	StaticArray<const ModelResource*> models;
	// Contains time foreach model
	StaticArray<float> modelsTime;


	// Temp vertices for animation
	// For each mesh, for each vertex
	StaticArray<StaticArray<Vertex5>*> tempVerts;

	// Current time of animation
	float currentTime;
	bool isPlaying;

private:
	// Init as dynamic (deformable) model
	void InitDynamic();
	// Update graphics arrays
	void GFXUpdate();

	int GetModelID(float time);

public:
	// Destroys allocated data
	~CVertexAnimated();

	// Play animation on this model
	void PlayAnimation(int animationIndex, float startTime = 0.0f);

public:
	void Init() override;
	void Update() override;
	void SetProperty(const String &key, const String &value) override;
};