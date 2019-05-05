#pragma once

#include "IComponent.h"
#include "CModel.h"
#include <Engine/ResourceManager/ModelResource.h>
#include <Engine/ResourceManager/VertexAnimatedResource.h>
#include <Engine/Rendering/IAnimatable.h>
#include <Engine/Rendering/VertexAnimation.h>

class CVertexAnimated : public CModel, public IAnimatable
{
	friend class ResourceManager;
	
	CLASSDECLARATION(CModel)

private:
	// Path to anim file
	String vertAnimListPath;

	// Array of vertex animations
	// Each contains all loaded models for interpolating and time foreach model
	StaticArray<VertexAnimation*> vertAnimations;

	// Temp vertices for animation
	// For each mesh, for each vertex
	StaticArray<StaticArray<Vertex5>*> tempVerts;

	// Current time of animation
	float	currentTime;
	// Index of current playing animation, -1 if not playing
	int		playingAnimIndex;
	bool	isPlaying;

private:
	// Init as dynamic (deformable) model
	void InitDynamic();
	// Update graphics arrays
	void GFXUpdate();

	void InitVertices(const StaticArray<MeshResource*> &baseMeshes);

public:
	// Empty constructor
	CVertexAnimated();
	// Destroys allocated data
	~CVertexAnimated();

	// Play animation on this model
	void	PlayAnimation(int animationIndex, float startTime = 0.0f) override;
	bool	IsPlaying() override;
	int		GetCurrentAnimation() override;
	float	GetAnimationLength(int animationIndex) override;

public:
	void Init() override;
	void Update() override;
	void SetProperty(const String &key, const String &value) override;
};