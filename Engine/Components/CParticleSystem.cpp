#include "CParticleSystem.h"
#include <Engine/Memory/Memory.h>
#include <Engine/Components/CCamera.h>

#include <algorithm>

// must include opengl to use specialized instanced rendering
#include <Engine/Rendering/OpenGL.h>

float CParticleSystem::quadVertices[] = 
{ 
	-0.5f,-0.5f, 0.0f,
	 0.5f,-0.5f, 0.0f,
	-0.5f, 0.5f, 0.0f,
	 0.5f, 0.5f, 0.0f,
};

#define MAX_PARTICLE_COUNT 10000

CLASSDEFINITION(IComponent, CParticleSystem)

void CParticleSystem::BindCamera(const CCamera * cam)
{
	this->cam = cam;
}

void CParticleSystem::Init()
{
	ASSERT(maxParticleCount > 0 && maxParticleCount <= MAX_PARTICLE_COUNT);

	// load shader
	particleShader.Load(shaderVert, shaderFrag);

	// allocate particales
	particles = (Particle*)SYSALLOCATOR.Allocate(sizeof(Particle) * maxParticleCount);
	lastUsedParticle = 0;

	// init particles
	for (UINT i = 0; i < maxParticleCount; i++)
	{
		particles[i].life = -1.0f;
		particles[i].camDistance = -1.0f;
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// generate buffers
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

	glGenBuffers(1, &positionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, maxParticleCount * sizeof(Vector4), NULL, GL_STREAM_DRAW);

	glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, maxParticleCount * sizeof(Color4), NULL, GL_STREAM_DRAW);
}

CParticleSystem::~CParticleSystem()
{
	SYSALLOCATOR.Free(particles);
}

UINT CParticleSystem::FindDeadParticle()
{
	for (UINT i = lastUsedParticle; i < maxParticleCount; i++)
	{
		if (particles[i].life < 0)
		{
			lastUsedParticle = i;
			return i;
		}
	}

	for (UINT i = 0; i < lastUsedParticle; i++)
	{
		if (particles[i].life < 0)
		{
			lastUsedParticle = i;
			return i;
		}
	}

	// all particles are not alive
	return 0;
}

void CParticleSystem::Update()
{ 
	UINT newparticles = (UINT)(Time::GetDeltaTime() * 10000.0f);
	if (newparticles > (UINT)(1.0f / 60.0f * 10000.0f))
	{
		newparticles = (UINT)(1.0f / 60.0f * 10000.0f);
	}

	for (UINT i = 0; i < newparticles; i++)
	{
		UINT particleIndex = FindDeadParticle();
		particles[particleIndex].life = 5.0f;
		particles[particleIndex].position = owner->GetTransform().GetPosition();
	
		float spread = 1.5f;
		Vector3 maindir = Vector3(0.0f, 10.0f, 0.0f);
		// Very bad way to generate a random direction; 
		Vector3 randomdir = Vector3(
			(rand() % 2000 - 1000.0f) / 1000.0f,
			(rand() % 2000 - 1000.0f) / 1000.0f,
			(rand() % 2000 - 1000.0f) / 1000.0f
		);

		particles[particleIndex].velocity = maindir + randomdir*spread;


		// Very bad way to generate a random color
		particles[particleIndex].color[0] = rand() % 256;
		particles[particleIndex].color[1] = rand() % 256;
		particles[particleIndex].color[2] = rand() % 256;
		particles[particleIndex].color[3] = (rand() % 256) / 3;

		particles[particleIndex].size = (rand() % 1000) / 2000.0f + 0.1f;
	}

	UINT particlesCount = Simulate();

	// if there is no alive particles
	if (particlesCount == 0)
	{
		return;
	}

	SortParticles();

	glBindVertexArray(vao);
	StoreData(particlesCount);
	ActivateShader();
	LoadAndDraw(particlesCount);
}

UINT CParticleSystem::Simulate()
{
	UINT particlesCount = 0;

	for (UINT i = 0; i < maxParticleCount; i++) {

		Particle& p = particles[i];

		// only alive particles are processed
		if (p.life > 0.0f)
		{
			p.life -= Time::GetDeltaTime();

			if (p.life > 0.0f)
			{
				p.velocity += Vector3(0.0f, -9.81f, 0.0f) * Time::GetDeltaTime();
				p.position += p.velocity * Time::GetDeltaTime();
				
				// length is sqr: only for comparing
				p.camDistance = (p.position - cam->GetPosition()).LengthSqr();

				// debug?
				positionsAndSize[4 * particlesCount + 0] = p.position[0];
				positionsAndSize[4 * particlesCount + 1] = p.position[1];
				positionsAndSize[4 * particlesCount + 2] = p.position[2];

				positionsAndSize[4 * particlesCount + 3] = p.position[3];

				colors[4 * particlesCount + 0] = p.color[0];
				colors[4 * particlesCount + 1] = p.color[1];
				colors[4 * particlesCount + 2] = p.color[2];
				colors[4 * particlesCount + 3] = p.color[3];
			}
			else
			{
				// particles that JUST died will be put at the end of the buffer in SortParticles();
				p.camDistance = -1.0f;
			}

			particlesCount++;
		}
	}

	return particlesCount;
}

void CParticleSystem::SortParticles()
{
	std::sort(&particles[0], &particles[maxParticleCount]);
}

void CParticleSystem::StoreData(UINT particlesCount)
{
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, maxParticleCount * sizeof(Vector4), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, particlesCount * sizeof(Vector4), positionsAndSize);

	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, maxParticleCount * sizeof(Color4), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, particlesCount * sizeof(Color4), colors);
}

void CParticleSystem::ActivateShader()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	particleShader.Use();

	const Transform &t = cam->GetOwner().GetTransform();
	particleShader.SetVec3("cameraUp", t.GetUp());
	particleShader.SetVec3("cameraRight", t.GetRight());

	Matrix4 viewProj = cam->GetProjectionMatrix(16, 9) * cam->GetViewMatrix();
	particleShader.SetMat4("viewProj", viewProj);
}

void CParticleSystem::LoadAndDraw(UINT particleCount)
{
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, (void*)0);

	glVertexAttribDivisor(0, 0); // 0 per quad (reuse same vertices)
	glVertexAttribDivisor(1, 1); // 1 position and size per quad
	glVertexAttribDivisor(2, 1); // 1 color per quad
	
	// instanced drawing
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particleCount);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void CParticleSystem::SetProperty(const String & key, const String & value)
{
}