#include "ResourceManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include <ImageLoading\stb_image.h>

#include <Engine/DataStructures/StaticArray.h>
#include <Engine/Math/Vector.h>
#include <Engine/Math/Triangle.h>
#include <Engine/Rendering/Texture.h>
#include <Engine/Rendering/Material.h>
#include <Engine/Rendering/Vertex.h>

#include "MeshResource.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

void ResourceManager::Init()
{
	meshResources.Init(128);

	modelResources.Init(32, 8);
	modelResources.DeclareHashFunction(String::StringHash);

	textureResources.Init(64, 8);
	textureResources.DeclareHashFunction(String::StringHash);
}

void ResourceManager::Unload()
{
	// clear data in array
	for (int i = 0; i < meshResources.GetSize(); i++)
	{
		delete meshResources[i];
	}

	for (int i = 0; i < meshColliderResources.GetSize(); i++)
	{
		delete meshColliderResources[i];
	}

	UINT size = modelResources.GetSize();
	for (UINT i = 0; i < size; i++)
	{
		delete modelResources[i];
	}
}

ResourceManager::~ResourceManager()
{ 
	Unload();
}

const TextureResource *ResourceManager::LoadTexture(char const *path)
{
	TextureResource *outTexture;

	// if already loaded
	if (textureResources.Find(path, outTexture))
	{
		return outTexture;
	}

	// load through stbi
	int width, height, chanNum;
	BYTE* data = (BYTE*)stbi_load(path, &width, &height, &chanNum, 0);

	// allocate data
	outTexture = new TextureResource(data, path, width, height, chanNum);

	// add to hash table
	textureResources.Add(path, outTexture);

	return outTexture;
}

const ModelResource *ResourceManager::LoadModel(const char *path)
{
	ModelResource *outModel;

	// if already loaded
	if (modelResources.Find(path, outModel))
	{
		return outModel;
	}

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	
	// check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		Logger::Print(importer.GetErrorString());
		return nullptr;
	}

	// allocate memory
	outModel = new ModelResource(path);

	// process ASSIMP's root node recursively
	ProcessModelNode(scene->mRootNode, scene, outModel);

	// add to hash table
	modelResources.Add(path, outModel);

	return outModel;
}

ResourceManager &ResourceManager::Instance()
{
	static ResourceManager instance;
	return instance;
}

void ResourceManager::ProcessModelNode(void *n, const void *s, ModelResource *outModel)
{
	aiNode *node = (aiNode*)n;
	aiScene *scene = (aiScene*)s;

	// get tranformations for a mesh relative to parent
	aiVector3D aipos;
	aiQuaternion aiquat;
	aiVector3D aiscale;
	node->mTransformation.Decompose(aiscale, aiquat, aipos);

	// convert
	Vector3 pos((float)aipos.x, (float)aipos.y, (float)aipos.z);
	Quaternion quat((float)aiquat.w, (float)aiquat.x, (float)aiquat.y, (float)aiquat.z);
	Vector3 scale((float)aiscale.x, (float)aiscale.y, (float)aiscale.z);

	// current node tranformations
	Transform currentTransform = Transform(pos, quat, scale);

	// for each mesh
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		// mesh to process
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

		// process data from mesh
		MeshResource *meshRes = ProcessMesh(mesh, scene, node->mName.C_Str(), currentTransform);

		// add mesh to model
		outModel->AddMesh(meshRes);
	}

	// for each child node
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		// process nodes recursively
		ProcessModelNode(node->mChildren[i], scene, outModel);
	}
}

MeshResource *ResourceManager::ProcessMesh(void *m, const void *s, const char* name, const Transform &transform)
{
	using namespace std;

	aiMesh *mesh = (aiMesh*)m;
	aiScene *scene = (aiScene*)s;

	// init
	StaticArray<Vertex5> vertices;
	vertices.Init(mesh->mNumVertices);

	// count indices count
	UINT indexCount = 0;
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		indexCount += mesh->mFaces[i].mNumIndices;
	}

	// init
	StaticArray<UINT> indices;
	indices.Init(indexCount);

	// init triangle array
	StaticArray<Triangle> triangles;
	triangles.Init(mesh->mNumFaces);


	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex5 vertex;

		// temp vector
		Vector3 vector;

		// positions
		vector[0] = mesh->mVertices[i].x;
		vector[1] = mesh->mVertices[i].y;
		vector[2] = mesh->mVertices[i].z;
		vertex.Position = vector;

		// normals
		vector[0] = mesh->mNormals[i].x;
		vector[1] = mesh->mNormals[i].y;
		vector[2] = mesh->mNormals[i].z;
		vertex.Normal = vector;

		// only one texture coords, there can be up to 8
		if (mesh->mTextureCoords[0])
		{
			Vector2 vec;
		
			vec[0] = mesh->mTextureCoords[0][i].x;
			vec[1] = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
		{
			vertex.TexCoords = Vector2(0.0f, 0.0f);
		}

		if (mesh->HasTangentsAndBitangents())
		{
			// tangent
			vector[0] = mesh->mTangents[i].x;
			vector[1] = mesh->mTangents[i].y;
			vector[2] = mesh->mTangents[i].z;
			vertex.Tangent = vector;

			// bitangent
			vector[0] = mesh->mBitangents[i].x;
			vector[1] = mesh->mBitangents[i].y;
			vector[2] = mesh->mBitangents[i].z;
			vertex.Bitangent = vector;
		}

		vertices[i] = vertex;
	}

	// index counter
	UINT iindex = 0;

	// foreach face
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		
		// foreach index
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices[iindex] = face.mIndices[j];
			iindex++;
		}

		// if face is triangle
		// and there weren't non triangle faces
		if (face.mNumIndices == 3 && !triangles.IsEmpty())
		{ 
			triangles[i].A = vertices[face.mIndices[0]].Position; 
			triangles[i].B = vertices[face.mIndices[1]].Position;
			triangles[i].C = vertices[face.mIndices[2]].Position;
		}
		else
		{
			Logger::Print("Non triangle was found when loading mesh. No collision data created.");
			triangles.Delete();
		}
	}

	// create resource
	MeshResource *resource = new MeshResource(name, transform, vertices, indices, triangles);

	// register pointer for deallocation
	meshResources.Push(resource);

	return resource;
}