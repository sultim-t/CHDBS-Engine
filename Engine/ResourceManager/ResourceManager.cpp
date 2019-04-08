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

void ResourceManager::Init()
{
	modelResources.Init(32, 8);
	modelResources.DeclareHashFunction(String::StringHash);

	textureResources.Init(64, 8);
	textureResources.DeclareHashFunction(String::StringHash);
}

void ResourceManager::Unload()
{
	for (int i = 0; i < meshColliderResources.GetSize(); i++)
	{
		delete meshColliderResources[i];
	}

	for (UINT i = 0; i < modelResources.GetSize(); i++)
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

const ModelResource *ResourceManager::LoadModel(const char * path)
{
	ModelResource *resultModel;

	// if already loaded
	if (modelResources.Find(path, resultModel))
	{
		return resultModel;
	}

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	// check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		Logger::Print(importer.GetErrorString());
		return nullptr;
	}

	// process assimp's nodes recursively
	ModelNode *rootNode = ProcessModelNode(scene->mRootNode, scene, nullptr);

	// allocate memory
	ModelHierarchy *hierarchy = new ModelHierarchy(rootNode, scene->mNumMeshes, scene->mNumAnimations);

	StaticArray<MeshResource*> &meshes = hierarchy->meshes;
	StaticArray<Animation*>	&animations = hierarchy->animations;

	// copy mesh from assimp
	for (UINT i = 0; i < scene->mNumMeshes; i++)
	{	
		aiMesh *sourceMesh = scene->mMeshes[i];

		// count indices count
		UINT indexCount = 0;
		for (UINT i = 0; i < sourceMesh->mNumFaces; i++)
		{
			indexCount += sourceMesh->mFaces[i].mNumIndices;
		}

		// allocate memory
		meshes[i] = new MeshResource(sourceMesh->mNumVertices, indexCount, sourceMesh->mNumFaces, sourceMesh->mNumBones);

		CopyMesh(sourceMesh, meshes[i]);

		// if there are bones
		if (meshes[i]->HasBones())
		{
			CopyBones(sourceMesh, hierarchy, meshes[i]);
		}
	}

	// copy animations from assimp
	for (UINT i = 0; i < scene->mNumAnimations; i++)
	{
		aiAnimation *from = scene->mAnimations[i];

		// allocate memory for animation
		animations[i] = new Animation(
			from->mName.C_Str(),
			(float)from->mDuration,
			(float)from->mTicksPerSecond,
			(int)from->mNumChannels);

		// allocate memory for animation nodes
		for (UINT j = 0; j < from->mNumChannels; j++)
		{
			aiNodeAnim *fromNode = from->mChannels[j];

			animations[i]->animationNodes[j] = new AnimationNode(
				fromNode->mNumPositionKeys, 
				fromNode->mNumRotationKeys,
				fromNode->mNumScalingKeys);
		}

		// fill data
		CopyAnimation(from, animations[i]);
	}

	// allocate
	resultModel = new ModelResource(path, hierarchy);

	// add to hash table
	modelResources.Add(path, resultModel);

	return resultModel;
}

void ResourceManager::CopyMesh(void *from, MeshResource *to)
{
	aiMesh *mesh = (aiMesh*)from;

	StaticArray<Vertex5>	&vertices	= to->vertices;
	StaticArray<UINT>		&indices	= to->indices;
	StaticArray<Triangle>	&triangles	= to->triangles;

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
		for (UINT j = 0; j < face.mNumIndices; j++)
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
}

void ResourceManager::CopyBones(void * from, ModelHierarchy * hierarchy, MeshResource * to)
{
	aiMesh *mesh = (aiMesh*)from;

	StaticArray<Bone> &bones = to->bones;

	// foreach bone
	for (UINT i = 0; i < mesh->mNumBones; i++)
	{
		aiBone *orig = mesh->mBones[i];
		const char *boneName = orig->mName.C_Str();

		// copy matrix
		Matrix4 m;
		for (int x = 0; x < 4; x++)
		{
			for (int y = 0; y < 4; y++)
			{
				m(x, y) = orig->mOffsetMatrix[x][y];
			}
		}

		int weightsCountInModel = (int)orig->mNumWeights;
		int weightsCount = weightsCountInModel > BONE_MAX_WEIGHTS ? BONE_MAX_WEIGHTS : weightsCountInModel;

		// find bone node and parent bone
		const ModelNode *boneNode = hierarchy->FindNode(boneName);
		const Bone *parentBone = nullptr;

		// if not a root node
		if (boneNode->parent != nullptr)
		{
			// get bone's parent node name
			const String &parentNodeName = boneNode->parent->GetName();

			// try to find bone with the same name
			for (UINT i = 0; i < mesh->mNumBones; i++)
			{
				// if names are equal
				if (parentNodeName == mesh->mBones[i]->mName.C_Str())
				{
					// save its address
					parentBone = &bones[i];
					break;
				}
			}
		}

		// create bone
		bones[i] = Bone(boneName, parentBone, boneNode, weightsCount, m);

		// if weights in model more they will be normalized
		float sum = 0.0f;

		// get sum
		for (int w = 0; w < weightsCount; w++)
		{
			sum += orig->mWeights[w].mWeight;
		}

		// set normalized weights
		for (int w = 0; w < weightsCount; w++)
		{
			VertexWeight vweight = VertexWeight(orig->mWeights[w].mVertexId, orig->mWeights[w].mWeight / sum);
			bones[i].SetWeight((int)w, vweight);
		}
	}
}

void ResourceManager::CopyAnimation(void *f, Animation *to)
{
	aiAnimation *from = (aiAnimation*)f;

	// allocate array for animation nodes
	StaticArray<AnimationNode*> &animNodes = to->animationNodes;
	
	// copy data
	for (UINT i = 0; i < from->mNumChannels; i++)
	{
		// source
		aiNodeAnim *source = from->mChannels[i];
		
		// destination
		StaticArray<AKeyPosition>	&positionKeys	= animNodes[i]->positionKeys;
		StaticArray<AKeyRotation>	&rotationKeys	= animNodes[i]->rotationKeys;
		StaticArray<AKeyScale>		&scaleKeys		= animNodes[i]->scaleKeys;

		// copy positions
		for (UINT k = 0; k < source->mNumPositionKeys; k++)
		{
			positionKeys[k].Time = (float)source->mPositionKeys[k].mTime;
		
			for (int j = 0; j < 3; j++)
			{
				positionKeys[k].Value[j] = source->mPositionKeys[k].mValue[j];
			}
		}

		// copy rotations
		for (UINT k = 0; k < source->mNumRotationKeys; k++)
		{
			rotationKeys[k].Time = (float)source->mRotationKeys[k].mTime;
			
			rotationKeys[k].Value[0] = source->mRotationKeys[k].mValue.w;
			rotationKeys[k].Value[1] = source->mRotationKeys[k].mValue.x;
			rotationKeys[k].Value[2] = source->mRotationKeys[k].mValue.y;
			rotationKeys[k].Value[3] = source->mRotationKeys[k].mValue.z;
		}

		// copy scales
		for (UINT k = 0; k < source->mNumScalingKeys; k++)
		{
			scaleKeys[k].Time = (float)source->mScalingKeys[k].mTime;
			
			for (int j = 0; j < 3; j++)
			{
				scaleKeys[k].Value[j] = source->mScalingKeys[k].mValue[j];
			}
		}
	}
}

ModelNode *ResourceManager::ProcessModelNode(void *n, const void *s, ModelNode *parentNode)
{
	aiNode *source = (aiNode*)n;
	aiScene *scene = (aiScene*)s;

	// copy transformation
	Matrix4 transformation;
	for (int x = 0; x < 4; x++)
	{
		for (int y = 0; y < 4; y++)
		{
			// engine is using transposed transformation matrices
			transformation(x, y) = source->mTransformation[y][x];
		}
	}

	// allocate data for model node
	ModelNode *node = new ModelNode(source->mName.C_Str(), parentNode, transformation, source->mNumChildren, source->mNumMeshes);

	// data to fill
	StaticArray<int>		&meshes		= node->meshes;
	StaticArray<ModelNode*> &childNodes = node->childNodes;

	// copy mesh indices
	for (UINT i = 0; i < source->mNumMeshes; i++)
	{
		meshes[i] = (int)source->mMeshes[i];
	}

	// copy child nodes
	for (UINT i = 0; i < source->mNumChildren; i++)
	{
		// process nodes recursively
		childNodes[i] = ProcessModelNode(source->mChildren[i], scene, node);
	}

	return node;
}

ResourceManager &ResourceManager::Instance()
{
	static ResourceManager instance;
	return instance;
}