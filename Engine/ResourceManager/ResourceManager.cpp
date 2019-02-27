#include "ResourceManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include <ImageLoading\stb_image.h>

#include <Engine/DataStructures/StaticArray.h>
#include <Engine/Math/Vector.h>
#include <Engine/Math/Triangle.h>
#include <Engine/Rendering/Texture.h>
#include <Engine/Components/CMesh.h>
#include <Engine/Components/CModel.h>
#include <Engine/Rendering/Material.h>

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
}

ResourceManager::~ResourceManager()
{
	// clear data in array
	for (int i = 0; i < meshResources.GetSize(); i++)
	{
		delete meshResources[i];
	}
}

UBYTE * ResourceManager::LoadTexture(char const * filename, int * width, int * height, int * comp, int req_comp)
{
	return stbi_load(filename, width, height, comp, req_comp);
}

void ResourceManager::DeleteTexture(void * address)
{
	stbi_image_free(address);
}

void ResourceManager::LoadModel(const char *path, CModel &outModel)
{
	using namespace std;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	
	// check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		Logger::Print(importer.GetErrorString());
		return;
	}

	// process ASSIMP's root node recursively
	ProcessModelNode(scene->mRootNode, scene, outModel);
}

ResourceManager & ResourceManager::Instance()
{
	static ResourceManager instance;
	return instance;
}

void ResourceManager::ProcessModelNode(void *n, const void *s, CModel &outModel)
{
	aiNode *node = (aiNode*)n;
	aiScene *scene = (aiScene*)s;
	
	// for each mesh
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

		Mesh &m = ProcessMesh(mesh, scene, outModel);

		if (node->mName.length != 0)
		{
			m.SetName(node->mName.C_Str());
		}

		// get tranformations for a mesh relative to parent
		// NOTE: parent transformation are not included (temporary)
		aiVector3D aipos;
		aiQuaternion aiquat;
		aiVector3D aiscale;
		node->mTransformation.Decompose(aiscale, aiquat, aipos);

		// convert
		Vector3 pos((float)aipos.x, (float)aipos.y, (float)aipos.z);
		Quaternion quat((float)aiquat.w, (float)aiquat.x, (float)aiquat.y, (float)aiquat.z);
		Vector3 scale((float)aiscale.x, (float)aiscale.y, (float)aiscale.z);

		// set
		Transform t = Transform(pos, quat, scale);
		m.SetTransform(t);

		outModel.meshes.push_back(m);
	}

	// for each child node
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessModelNode(node->mChildren[i], scene, outModel);
	}
}

Mesh ResourceManager::ProcessMesh(void *m, const void *s, CModel &outModel)
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
	MeshResource *resource = new MeshResource(mesh->mName.C_Str(), vertices, indices, triangles);
	meshResources.Push(resource);

	return Mesh(resource);
}