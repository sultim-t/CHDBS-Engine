#include "ResourceManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include <ImageLoading\stb_image.h>

#include <Engine/DataStructures/StaticArray.h>
#include <Engine/Math/Vector.h>
#include <Engine/Math/Triangle.h>
#include <Engine/Rendering/Texture.h>
#include <Engine/Rendering/Material.h>
#include <Engine/Rendering/Vertex.h>
#include <Engine/Rendering/Bone.h>
#include <Engine/Rendering/Animation.h>
#include <Engine/Rendering/AnimationNode.h>
#include <Engine/Rendering/AnimationKey.h>
#include <Engine/Rendering/AnimationVertex.h>

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

	models.Init(32, 8);
	models.DeclareHashFunction(String::StringHash);

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

	for (UINT i = 0; i < modelResources.GetSize(); i++)
	{
		delete modelResources[i];
	}

	for (UINT i = 0; i < models.GetSize(); i++)
	{
		delete models[i];
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

const ModelHierarchy *ResourceManager::LoadModelF(const char * path)
{
	ModelHierarchy *outModel;

	// if already loaded
	if (models.Find(path, outModel))
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
	StaticArray<MeshResource*> meshes;
	StaticArray<Animation*>	animations;
	meshes.Init(scene->mNumMeshes);
	animations.Init(scene->mNumAnimations);

	// copy mesh from assimp
	for (UINT i = 0; i < scene->mNumMeshes; i++)
	{
		CopyMesh(scene->mMeshes[i], meshes[i]);
	}

	// copy animations from assimp
	for (UINT i = 0; i < scene->mNumAnimations; i++)
	{
		CopyAnimation(scene->mAnimations[i], animations[i]);
	}

	// process assimp's nodes recursively
	ModelNode *rootNode = ProcessModelNodeF(scene->mRootNode, scene, nullptr);

	// allocate and add to hash table
	outModel = new ModelHierarchy(rootNode, meshes, animations);
	models.Add(path, outModel);

	return outModel;
}

void ResourceManager::CopyMesh(void *from, MeshResource *to)
{
	aiMesh *mesh = (aiMesh*)from;

	// count indices count
	UINT indexCount = 0;
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		indexCount += mesh->mFaces[i].mNumIndices;
	}

	// init vertices array
	StaticArray<Vertex5> vertices;
	vertices.Init(mesh->mNumVertices);

	// init indices array
	StaticArray<UINT> indices;
	indices.Init(indexCount);

	// init triangle array
	StaticArray<Triangle> triangles;
	triangles.Init(mesh->mNumFaces);

	// init bones array
	StaticArray<Bone> bones;
	bones.Init(mesh->mNumBones);

	for (UINT i = 0; i < mesh->mNumVertices; i++)
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
	for (UINT i = 0; i < mesh->mNumFaces; i++)
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

	// foreach bone
	for (UINT i = 0; i < mesh->mNumBones; i++)
	{
		aiBone *orig = mesh->mBones[i];
		Matrix4 m;

		// copy matrix
		for (int x = 0; x < 4; x++)
		{
			for (int y = 0; y < 4; y++)
			{
				m(x, y) = orig->mOffsetMatrix[x][y];
			}
		}
		
		// create bone
		bones[i] = Bone(orig->mNumWeights, m);

		// foreach weight in bone
		for (UINT w = 0; w < orig->mNumWeights; w++)
		{
			VertexWeight vweight = VertexWeight(orig->mWeights[w].mVertexId, orig->mWeights[w].mWeight);
			bones[i].SetWeight((int)i, vweight);
		}
	}

	to = new MeshResource(NULL,vertices, indices, triangles, bones);

	// register pointer for deallocation
	meshResources.Push(to);
}

void ResourceManager::CopyAnimation(void *f, void *t)
{
	aiAnimation *from = (aiAnimation*)f;
	Animation *to = (Animation*)t;

	// allocate array for animation nodes
	StaticArray<AnimationNode*> animNodes;
	animNodes.Init(from->mNumChannels);
	
	// copy data
	for (UINT i = 0; i < from->mNumChannels; i++)
	{
		aiNodeAnim *source = from->mChannels[i];
		
		// init data to set
		StaticArray<AKeyPosition>	positionKeys;
		StaticArray<AKeyRotation>	rotationKeys;
		StaticArray<AKeyScale>		scaleKeys;
		positionKeys.Init((UINT)source->mNumPositionKeys);
		rotationKeys.Init((UINT)source->mNumRotationKeys);
		scaleKeys	.Init((UINT)source->mNumScalingKeys);

		for (UINT k = 0; k < source->mNumPositionKeys; k++)
		{
			positionKeys[k].Time = (float)source->mPositionKeys[k].mTime;
		
			for (int j = 0; j < 3; j++)
			{
				positionKeys[k].Value[j] = source->mPositionKeys[k].mValue[j];
			}
		}

		for (UINT k = 0; k < source->mNumRotationKeys; k++)
		{
			rotationKeys[k].Time = (float)source->mRotationKeys[k].mTime;
			
			rotationKeys[k].Value[0] = source->mRotationKeys[k].mValue.w;
			rotationKeys[k].Value[1] = source->mRotationKeys[k].mValue.x;
			rotationKeys[k].Value[2] = source->mRotationKeys[k].mValue.y;
			rotationKeys[k].Value[3] = source->mRotationKeys[k].mValue.z;
		}

		for (UINT k = 0; k < source->mNumScalingKeys; k++)
		{
			scaleKeys[k].Time = (float)source->mScalingKeys[k].mTime;
			
			for (int j = 0; j < 3; j++)
			{
				scaleKeys[k].Value[j] = source->mScalingKeys[k].mValue[j];
			}
		}

		// set
		animNodes[i] = new AnimationNode(positionKeys, rotationKeys, scaleKeys);
	}

	// allocate animation
	to = new Animation((float)from->mDuration, (float)from->mTicksPerSecond, animNodes);
}

ModelNode *ResourceManager::ProcessModelNodeF(void *n, const void *s, ModelNode *parentNode)
{
	aiNode *source = (aiNode*)n;
	aiScene *scene = (aiScene*)s;

	// allocate data to fill
	Matrix4 transformation;

	StaticArray<int> meshes;
	meshes.Init(source->mNumMeshes);

	StaticArray<ModelNode*> childNodes;
	childNodes.Init(source->mNumChildren);

	// copy transformation
	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			transformation(x, y) = source->mTransformation[x][y];
		}
	}

	// copy mesh indices
	for (unsigned int i = 0; i < source->mNumMeshes; i++)
	{
		meshes[i] = source->mMeshes[i];
	}

	ModelNode *node = new ModelNode(parentNode, childNodes, transformation, meshes);

	// copy child nodes
	for (unsigned int i = 0; i < source->mNumChildren; i++)
	{
		// process nodes recursively
		childNodes[i] = ProcessModelNodeF(source->mChildren[i], scene, node);
	}

	return node;
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