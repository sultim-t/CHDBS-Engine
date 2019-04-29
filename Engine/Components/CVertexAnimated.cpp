#include "CVertexAnimated.h"

#include <Engine/Math/Vector.h>
#include <Engine/Rendering/OpenGL.h>
#include <Engine/Systems/RenderingSystem.h>
#include <Engine/ResourceManager/ResourceManager.h>
#include <Engine/ResourceManager/MeshResource.h>

CLASSDEFINITION(CModel, CVertexAnimated)

void CVertexAnimated::Init()
{
	// load vertex animation resource
	vertAnim = ResourceManager::Instance().LoadVertexAnimated(vertAnimPath);

	// load all models in this resource
	auto &nodes = vertAnim->GetAnimationNodes();
	models.Init(nodes.GetSize());
	modelsTime.Init(nodes.GetSize());

	for (UINT i = 0; i < nodes.GetSize(); i++)
	{
		models[i] = ResourceManager::Instance().LoadModel(nodes[i].ModelPath);
		modelsTime[i] = nodes[i].Time;
	}

	// set first as main model
	modelResource = models[0];

	// get meshes from main model to copy data
	const StaticArray<MeshResource*> &baseMeshes = modelResource->GetHierarchy().GetMeshes();
	UINT meshesCount = baseMeshes.GetSize();

	// init array for each mesh
	tempVerts.Init(meshesCount);

	for (UINT meshIndex = 0; meshIndex < meshesCount; meshIndex++)
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

	// init 
	vaos.Init(meshesCount);
	vbos.Init(meshesCount);
	ibos.Init(meshesCount);

	// init for rendering
	InitDynamic();

	// update once to buffer data
	GFXUpdate();
}

CVertexAnimated::~CVertexAnimated()
{ 
	// delete all arrays
	for (UINT i = 0; i < tempVerts.GetSize(); i++)
	{
		delete tempVerts[i];
	}
}

void CVertexAnimated::PlayAnimation(int animationIndex, float startTime)
{
	// enable animation
	isPlaying = true;
	// reset time
	currentTime = startTime;
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

void CVertexAnimated::Update()
{
	// if no animation is set, then ignore
	if (!isPlaying)
	{
		return;
	}

	if (currentTime >= vertAnim->GetDuration())
	{
		// animation ended
		isPlaying = false;
		return;

		// TODO: several types of animations: clamped, infinite, mirror etc
		// currentTime = Mod(currentTime, vertAnim->GetDuration());
	}

	// get model index in array
	int modelId = GetModelID(currentTime);
	int nextModelId = modelId + 1;

	if ((UINT)nextModelId >= models.GetSize())
	{
		nextModelId = 0;
	}

	UINT meshesCount = tempVerts.GetSize();
	const StaticArray<MeshResource*> &prevMeshes = models[modelId]->GetHierarchy().GetMeshes();
	const StaticArray<MeshResource*> &nextMeshes = models[nextModelId]->GetHierarchy().GetMeshes();

	// update each mesh
	for (UINT meshIndex = 0; meshIndex < meshesCount; meshIndex++)
	{
		StaticArray<Vertex5> &targetVerts = *tempVerts[meshIndex];

		// get previous mesh
		const StaticArray<Vertex5> &prevVerts = prevMeshes[meshIndex]->GetVertices();

		// get next mesh
		const StaticArray<Vertex5> &nextVerts = nextMeshes[meshIndex]->GetVertices();

		// get interpolation factor
		float t = (currentTime - modelsTime[modelId]) / (modelsTime[nextModelId] - modelsTime[modelId]);

		// interpolate between prev and next
		for (UINT i = 0; i < prevVerts.GetSize(); i++)
		{
			targetVerts[i].Position = Vector3::Lerp(prevVerts[i].Position, nextVerts[i].Position, t);
			targetVerts[i].Normal = Vector3::Lerp(prevVerts[i].Normal, nextVerts[i].Normal, t);
		}
	}

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

int CVertexAnimated::GetModelID(float time)
{
	for (UINT i = 0; i < modelsTime.GetSize() - 1; i++)
	{
		if (modelsTime[i] <= time && time < modelsTime[i + 1])
		{
			return (int)i;
		}
	}

	return 0;
}

#define PROPERTY_KEY_VERTANIMPATH	"VertexAnimatedPath"

void CVertexAnimated::SetProperty(const String & key, const String & value)
{
	if (key == PROPERTY_KEY_VERTANIMPATH)
	{
		vertAnimPath = value;
		return;
	}

	CModel::SetProperty(key, value);
}
