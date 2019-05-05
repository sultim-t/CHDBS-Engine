#include "CVertexAnimated.h"

#include <Engine/Math/Vector.h>
#include <Engine/Rendering/OpenGL.h>
#include <Engine/Systems/RenderingSystem.h>
#include <Engine/ResourceManager/ResourceManager.h>
#include <Engine/ResourceManager/MeshResource.h>

CLASSDEFINITION(CModel, CVertexAnimated)

CVertexAnimated::CVertexAnimated()
{ }

void CVertexAnimated::Init()
{
	// load list
	const ListResource *animationList = ResourceManager::Instance().LoadList(vertAnimListPath);
	const StaticArray<String> &lines = animationList->GetLines();

	ASSERT(lines.GetSize() != 0);

	// load each animation in this list
	vertAnimations.Init(lines.GetSize());
	for (UINT i = 0; i < lines.GetSize(); i++)
	{
		vertAnimations[i] = new VertexAnimation();
		vertAnimations[i]->Init(lines[i]);
	}

	// set first as main model
	modelResource = vertAnimations[0]->GetVertexAnimationResource()->GetAnimationNodes()[0].Value;

	// get meshes from main model
	const StaticArray<MeshResource*> &baseMeshes = modelResource->GetHierarchy().GetMeshes();
	// copy data
	InitVertices(baseMeshes);

	// init 
	UINT meshesCount = baseMeshes.GetSize();
	vaos.Init(meshesCount);
	vbos.Init(meshesCount);
	ibos.Init(meshesCount);

	// init for rendering
	InitDynamic();

	// update once to buffer data
	GFXUpdate();

	// bind shader only for main model
	BindShader(shader);
}

void CVertexAnimated::InitVertices(const StaticArray<MeshResource*>& baseMeshes)
{
	int meshesCount = baseMeshes.GetSize();

	// init array for each mesh
	tempVerts.Init(meshesCount);

	for (int meshIndex = 0; meshIndex < meshesCount; meshIndex++)
	{
		const StaticArray<Vertex5> &baseVerts = baseMeshes[meshIndex]->GetVertices();
		tempVerts[meshIndex] = new StaticArray<Vertex5>();

		StaticArray<Vertex5> &targetVerts = *tempVerts[meshIndex];

		// init temp verts
		targetVerts.Init(baseVerts.GetSize());

		for (UINT i = 0; i < baseVerts.GetSize(); i++)
		{
			targetVerts[i] = baseVerts[i];
		}
	}
}

void CVertexAnimated::InitDynamic()
{
	const StaticArray<MeshResource*> &meshes = modelResource->GetHierarchy().GetMeshes();
	UINT meshesCount = modelResource->GetMeshesCount();

	for (UINT i = 0; i < meshesCount; i++)
	{
		// init for rendering, for dynamic draw
		InitMesh(meshes[i], vaos[i], vbos[i], ibos[i], true, true);
	}
}

CVertexAnimated::~CVertexAnimated()
{ 
	// delete all arrays
	for (UINT i = 0; i < tempVerts.GetSize(); i++)
	{
		delete tempVerts[i];
	}

	// delete animations
	for (UINT i = 0; i < vertAnimations.GetSize(); i++)
	{
		delete vertAnimations[i];
	}
}

void CVertexAnimated::Update()
{
	// if no animation is set, then ignore
	if (!isPlaying)
	{
		return;
	}

	ASSERT(playingAnimIndex >= 0 && playingAnimIndex < (int)vertAnimations.GetSize());

	// update temp vertices according to current time
	// it will return false, if last frame was animated
	isPlaying = vertAnimations[playingAnimIndex]->Animate(tempVerts, currentTime);

	// load model to gpu
	GFXUpdate();

	// update time for animation
	currentTime += Time::GetDeltaTime();
}

void CVertexAnimated::GFXUpdate()
{
	UINT meshCount = tempVerts.GetSize();

	for (UINT meshIndex = 0; meshIndex < meshCount; meshIndex++)
	{
		// bind current mesh
		glBindVertexArray(vaos[meshIndex]);
		// bind mesh's buffer
		glBindBuffer(GL_ARRAY_BUFFER, vbos[meshIndex]);

		// update data in buffer
		glBufferSubData(GL_ARRAY_BUFFER, 0, tempVerts[meshIndex]->GetSize() * sizeof(Vertex5), tempVerts[meshIndex]->GetArray());

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}

void CVertexAnimated::PlayAnimation(int animationIndex, float startTime)
{
	// enable animation
	isPlaying = true;
	playingAnimIndex = animationIndex;

	// reset time
	currentTime = startTime;
}

bool CVertexAnimated::IsPlaying()
{
	return isPlaying;
}

int CVertexAnimated::GetCurrentAnimation()
{
	return isPlaying ? playingAnimIndex : -1;
}

float CVertexAnimated::GetAnimationLength(int animationIndex)
{
	return 0.0f;
}

#define PROPERTY_KEY_VERTANIMPATH	"VertexAnimationListPath"

void CVertexAnimated::SetProperty(const String & key, const String & value)
{
	if (key == PROPERTY_KEY_VERTANIMPATH)
	{
		vertAnimListPath = value;
		return;
	}

	CModel::SetProperty(key, value);
}