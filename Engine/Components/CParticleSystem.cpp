#include "CParticleSystem.h"
#include <Engine/Memory/Memory.h>
#include <Engine/Components/CCamera.h>
#include <Engine/Systems/RenderingSystem.h>
#include <Engine/Base/Random.h>

#include <algorithm>

// must include opengl to use specialized instanced rendering
#include <Engine/Rendering/OpenGL.h>


#define PROPERTY_KEY_VERTPATH	"vertPath"
#define PROPERTY_KEY_FRAGPATH	"fragPath"

#define PROPERTY_KEY_MAXCOUNT	"maxCount"

#define PROPERTY_KEY_VEL		"vel"
#define PROPERTY_KEY_VELRAND	"velRand"
#define PROPERTY_KEY_VELWORLD	"isWorldVelocity"

#define PROPERTY_KEY_COLR		"colr"
#define PROPERTY_KEY_COLG		"colg"
#define PROPERTY_KEY_COLB		"colb"
#define PROPERTY_KEY_COLA		"cola"
#define PROPERTY_KEY_COLRAND	"colRand"

#define PROPERTY_KEY_SIZE		"size"
#define PROPERTY_KEY_SIZERAND	"sizeRand"

#define PROPERTY_KEY_ROT		"rotation"
#define PROPERTY_KEY_ROTRAND	"rotationRand"

#define PROPERTY_KEY_LIFE		"lifetime"
#define PROPERTY_KEY_LIFERAND	"lifetimeRand"

#define PROPERTY_KEY_GRAVMULT	"gravMult"
#define PROPERTY_KEY_GRAVRAND	"gravMultRand"

#define PROPERTY_KEY_EMITRATE	"emitRate"
#define PROPERTY_KEY_LOOPING	"isLooping"


float CParticleSystem::quadVertices[] = 
{ 
	-0.5f,-0.5f, 0.0f,
	 0.5f,-0.5f, 0.0f,
	-0.5f, 0.5f, 0.0f,
	 0.5f, 0.5f, 0.0f,
};

#define MAX_PARTICLE_COUNT 10000

CLASSDEFINITION(IComponent, CParticleSystem)

void CParticleSystem::Emit(UINT count)
{
	Vector3 tempVel = startVelocity;
	Vector3 tempVelR = velocityRandomness;
	
	if (!isWorldVelocity)
	{
		// if velocity is in local space
		// transform it to world
		tempVel = transform->DirectionFromLocal(tempVel);
		tempVelR =  transform->DirectionFromLocal(tempVelR);
	}

	for (UINT i = 0; i < count; i++)
	{
		Vector3 vel = tempVel + Random::GetInsideBox(tempVelR);

		float life = startLifetime + Random::GetFloat(-lifetimeRandomness, lifetimeRandomness);
		float rot = startRotation + Random::GetFloat(-rotationRandomness, rotationRandomness);
		float size = startSize + Random::GetFloat(-sizeRandomness, sizeRandomness);

		GenerateParticle(transform->GetPosition(), vel, startColor, life, rot, size);
	}
}

void CParticleSystem::BindCamera(const CCamera * cam)
{
	this->cam = cam;
}

void CParticleSystem::Render()
{
	// if there is no alive particles
	if (particleCount == 0)
	{
		return;
	}

	// sort particles to prepare for rendering
	SortParticles();

	glBindVertexArray(vao);
	StoreData();
	ActivateShader();
	LoadAndDraw();
}

void CParticleSystem::Init()
{
	ASSERT(maxParticleCount > 0 && maxParticleCount <= MAX_PARTICLE_COUNT);

	// load shader
	particleShader.Load(shaderVert, shaderFrag);

	// allocate
	particles = (Particle*)SYSALLOCATOR.Allocate(sizeof(Particle) * maxParticleCount);
	positionsAndSizes = (Vector4*)SYSALLOCATOR.Allocate(sizeof(Vector4) * maxParticleCount);
	colors = (Color4*)SYSALLOCATOR.Allocate(sizeof(Color4) * maxParticleCount);

	lastUsedParticle = 0;
	cam = nullptr;
	transform = &GetOwner().GetTransform();

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

	// register particle system to render
	RenderingSystem::Instance().Register(this);
}

CParticleSystem::~CParticleSystem()
{
	SYSALLOCATOR.Free(particles);
	SYSALLOCATOR.Free(positionsAndSizes);
	SYSALLOCATOR.Free(colors);
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

	// all particles are alive
	// return next?
	lastUsedParticle = lastUsedParticle + 1;
	return lastUsedParticle;
}

void CParticleSystem::GenerateParticle(const Vector3 &pos, const Vector3 &vel, const Color4 &col, float life, float rot, float size)
{
	UINT particleIndex = FindDeadParticle();
	particles[particleIndex].position = pos;
	particles[particleIndex].velocity = vel;
	particles[particleIndex].color = col;
	particles[particleIndex].life = life;
	particles[particleIndex].rotation = rot;
	particles[particleIndex].size = size;
}

void CParticleSystem::Update()
{ 

	Simulate();
}

void CParticleSystem::Simulate()
{
	UINT count = 0;

	for (UINT i = 0; i < maxParticleCount; i++) 
	{
		Particle &p = particles[i];

		// only alive particles are processed
		if (p.life > 0.0f)
		{
			p.life -= Time::GetDeltaTime();

			if (p.life > 0.0f)
			{
				float g = gravityMultiplier + Random::GetFloat(-gravityMultRandomness, gravityMultRandomness);

				p.velocity += Vector3(0.0f, -9.81f, 0.0f) * g * Time::GetDeltaTime();
				p.position += p.velocity * Time::GetDeltaTime();
				
				positionsAndSizes[count][0] = p.position[0];
				positionsAndSizes[count][1] = p.position[1];
				positionsAndSizes[count][2] = p.position[2];
				positionsAndSizes[count][3] = p.size;

				colors[count] = p.color;
			}
			else
			{
				// particles that JUST died will be put at the end of the buffer in SortParticles();
				p.camDistance = -1.0f;
			}

			count++;
		}
	}

	particleCount = count;
}

void CParticleSystem::SortParticles()
{
	for (UINT i = 0; i < particleCount; i++)
	{
		// length is sqr: only for comparing
		particles[i].camDistance = (particles[i].position - cam->GetPosition()).LengthSqr();
	}

	std::sort(&particles[0], &particles[maxParticleCount]);
}

void CParticleSystem::StoreData()
{
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	glBufferData(GL_ARRAY_BUFFER, maxParticleCount * sizeof(Vector4), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, particleCount * sizeof(Vector4), positionsAndSizes);

	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, maxParticleCount * sizeof(Color4), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, particleCount * sizeof(Color4), colors);
}

void CParticleSystem::ActivateShader()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	particleShader.Use();

	const Transform &t = cam->GetOwner().GetTransform();
	particleShader.SetVec3("cameraUp", t.GetUp());
	particleShader.SetVec3("cameraRight", t.GetRight());

	Matrix4 viewProj = cam->GetViewMatrix() * cam->GetProjectionMatrix();
	particleShader.SetMat4("viewProj", viewProj);
}

void CParticleSystem::LoadAndDraw()
{
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	// store colors as integer type => normalized = GL_TRUE
	glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, (void*)0);

	glVertexAttribDivisor(0, 0); // 0 per quad (reuse same vertices)
	glVertexAttribDivisor(1, 1); // 1 position and size per quad
	glVertexAttribDivisor(2, 1); // 1 color per quad
	
	// instanced drawing
	// draw first particles in array
	// dead particles are in the end => will not be loaded and drawn
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particleCount);

	// to default
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	particleShader.Stop();
	glDisable(GL_BLEND);
}

void CParticleSystem::SetProperty(const String &key, const String &value)
{
	if (key == PROPERTY_KEY_VERTPATH)
	{
		shaderVert = value;
	}
	else if (key == PROPERTY_KEY_FRAGPATH)
	{
		shaderFrag = value;
	}
	else if (key == PROPERTY_KEY_MAXCOUNT)
	{
		maxParticleCount = (UINT)value.ToInt();
	}
	else if (key == PROPERTY_KEY_VEL)
	{
		startVelocity = value.ToVector3();
	}
	else if (key == PROPERTY_KEY_VELRAND)
	{
		velocityRandomness = value.ToVector3();
	}
	else if (key == PROPERTY_KEY_VELWORLD)
	{
		isWorldVelocity = value.ToBool();
	}
	else if (key == PROPERTY_KEY_COLR)
	{
		startColor[0] = (UBYTE)value.ToInt();
	}
	else if (key == PROPERTY_KEY_COLG)
	{
		startColor[1] = (UBYTE)value.ToInt();
	}
	else if (key == PROPERTY_KEY_COLB)
	{
		startColor[2] = (UBYTE)value.ToInt();
	}
	else if (key == PROPERTY_KEY_COLA)
	{
		startColor[3] = (UBYTE)value.ToInt();
	}
	else if (key == PROPERTY_KEY_COLRAND)
	{
		colorRandomness = value.ToFloat();
	}
	else if (key == PROPERTY_KEY_SIZE)
	{
		startSize = value.ToFloat();
	}
	else if (key == PROPERTY_KEY_SIZERAND)
	{
		sizeRandomness = value.ToFloat();
	}
	else if (key == PROPERTY_KEY_ROT)
	{
		startRotation = value.ToFloat();
	}
	else if (key == PROPERTY_KEY_ROTRAND)
	{
		rotationRandomness = value.ToFloat();
	}
	else if (key == PROPERTY_KEY_LIFE)
	{
		startLifetime = value.ToFloat();
	}
	else if (key == PROPERTY_KEY_LIFERAND)
	{
		lifetimeRandomness = value.ToFloat();
	}
	else if (key == PROPERTY_KEY_GRAVMULT)
	{
		gravityMultiplier = value.ToFloat();
	}
	else if (key == PROPERTY_KEY_GRAVRAND)
	{
		gravityMultRandomness = value.ToFloat();
	}
	else if (key == PROPERTY_KEY_EMITRATE)
	{
		emitRate = value.ToFloat();
	}
	else if (key == PROPERTY_KEY_LOOPING)
	{
		isLooping = value.ToBool();
	}
	else
	{
		Logger::Print("Wrong particle system property");
	}
}