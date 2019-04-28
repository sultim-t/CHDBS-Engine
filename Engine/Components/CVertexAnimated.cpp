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

	// init temp verts
	// only for first mesh (for now)
	tempVerts.Init(modelResource->GetHierarchy().GetMeshes()[0]->GetVertices().GetSize());

	// init 
	UINT meshesCount = modelResource->GetMeshesCount();
	vaos.Init(meshesCount);
	vbos.Init(meshesCount);
	ibos.Init(meshesCount);

	// init for rendering
	InitDynamic();
}

CVertexAnimated::~CVertexAnimated()
{ }

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
	if (currentTime > vertAnim->GetDuration())
	{
		currentTime = Mod(currentTime, vertAnim->GetDuration());
	}

	// get model index in array
	int modelId = GetModelID(currentTime);

	MeshResource *prevMesh = models[modelId]->GetHierarchy().GetMeshes()[0];
	const StaticArray<Vertex5> &prevVerts = prevMesh->GetVertices();
	
	MeshResource *nextMesh = models[modelId + 1]->GetHierarchy().GetMeshes()[0];
	const StaticArray<Vertex5> &nextVerts = nextMesh->GetVertices();

	float t = (currentTime - modelsTime[modelId]) / (modelsTime[modelId + 1] - modelsTime[modelId]);

	for (UINT i = 0; i < prevVerts.GetSize(); i++)
	{
		tempVerts[i].Position = Vector3::Lerp(prevVerts[i].Position, nextVerts[i].Position, t);
		tempVerts[i].Normal = Vector3::Lerp(prevVerts[i].Normal, nextVerts[i].Normal, t);
	}

	// load model to gpu
	GFXUpdate();

	// update time for animation
	currentTime += Time::GetDeltaTime();
}

void CVertexAnimated::GFXUpdate()
{
	// bind current mesh
	glBindVertexArray(vaos[0]);
	// bind mesh's buffer
	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);

	// update data in buffer
	glBufferSubData(GL_ARRAY_BUFFER, 0, tempVerts.GetSize() * sizeof(Vertex5), tempVerts.GetArray());

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

int CVertexAnimated::GetModelID(float time)
{
	for (UINT i = 0; i < modelsTime.GetSize() - 1; i++)
	{
		if (modelsTime[i] <= time && time < modelsTime[i])
		{
			return i;
		}
	}
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
