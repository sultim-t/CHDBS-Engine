#pragma once
#include <Engine/Components/IComponent.h>
#include <Engine/Rendering/Particle.h>
#include <Engine/Rendering/Shader.h>

class CParticleSystem : public IComponent
{
	CLASSDECLARATION(CParticleSystem)

public:
	UINT vao;
	UINT vertexBuffer;
	UINT positionBuffer;
	UINT colorBuffer;
	
	UINT lastUsedParticle = 0;
	UINT maxParticleCount = 0;

	Vector3 startVelocity;
	float velocityRandomness;

	float startLifetime;
	float lifetimeRandomness;

	float emitRate;
	bool isLooping;

	// array of particles
	Particle	*particles;
	// array of positions and sizes
	Vector4		*positionsAndSize;
	Color4		*colors;

	static float quadVertices[];

	// special shader for rendering particles
	Shader particleShader;
	String shaderVert, shaderFrag;

	// store pointer to camera
	// for sorting and aligning
	const CCamera *cam;

private:
	UINT FindDeadParticle();

	UINT Simulate();
	void SortParticles();
	void StoreData(UINT particlesCount);
	void ActivateShader();
	void LoadAndDraw(UINT particlesCount);

public:
	~CParticleSystem();

	// Start emit particles with current emit rate
	void Start();
	// Emit particles
	void Emit(UINT count);
	// Bind camera to render
	void BindCamera(const CCamera *cam);

public:
	void Init() override;
	void Update() override;

	void SetProperty(const String &key, const String &value) override;
};