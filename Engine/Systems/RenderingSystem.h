#pragma once
#include "ISystem.h"

#include <Engine/DataStructures/StaticArray.h>
#include <Engine/DataStructures/DynamicArray.h>
#include <Engine/DataStructures/LinkedList.h>
#include <Engine/DataStructures/HashTable.h>

#include <Engine/Components/Mesh.h>
#include <Engine/Components/CLight.h>
#include <Engine/Components/CParticleSystem.h>
#include <Engine/Rendering/Material.h>
#include <Engine/Rendering/Texture.h>
#include <Engine/Rendering/FramebufferTexture.h>

class RenderingSystem : public ISystem
{
public:
	// References to arrays with data in scene
	const DynamicArray<CCamera*>			*cameras;
	const DynamicArray<CLight*>				*lights;
	const DynamicArray<CModel*>				*models;
	const DynamicArray<CParticleSystem*>	*particleSystems;

	// main shadowmap
	FramebufferTexture shadowMap;
	// shader for shadowmapping
	Shader depthShader;

private:
	RenderingSystem();
	~RenderingSystem();

	RenderingSystem(RenderingSystem&) = delete;
	RenderingSystem(RenderingSystem&&) = delete;
	RenderingSystem &operator=(const RenderingSystem&) = delete;

	// Draw mesh with given vao
	void DrawMesh(UINT vao, UINT indicesCount);

	void DrawSkybox(const Matrix4 &viewMatrix, const Matrix4 &projMatrix);

	// Generate shadowmap to framebuffer, according to camera frustum to fit entire shadowmap
	void CreateShadowMap(const CLight &light, const Frustum &frustumForShadowmap, FramebufferTexture &shadowMap);

public:
	// Init structures
	void Init() override;
	// Render frame
	void Update() override;

	// Get instance of system
	static RenderingSystem &Instance();

	void Register(const DynamicArray<CCamera*>			*cameras);
	void Register(const DynamicArray<CLight*>			*lights);
	void Register(const DynamicArray<CModel*>			*models);
	void Register(const DynamicArray<CParticleSystem*>	*particleSystems);

	// Unregister rigidbodies and colliders
	void Reset();
};