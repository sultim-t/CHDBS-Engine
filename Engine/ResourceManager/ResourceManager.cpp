#include "ResourceManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include <ImageLoading/stb_image.h>

#include <Engine/DataStructures/StaticArray.h>
#include <Engine/Math/Vector.h>
#include <Engine/Math/Triangle.h>
#include <Engine/Rendering/Texture.h>
#include <Engine/Rendering/Material.h>
#include <Engine/Rendering/Vertex.h>
#include <Engine/Rendering/Bone.h>
#include <Engine/Rendering/Skeleton.h>
#include <Engine/Rendering/Animation.h>
#include <Engine/Rendering/AnimationNode.h>
#include <Engine/Rendering/AnimationKey.h>

#include "MeshResource.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <fstream>
#include <sstream>
#include <iostream>

#include <Engine/TEMP/tinyxml2/tinyxml2.h>

ResourceManager &ResourceManager::Instance()
{
	static ResourceManager instance;
	return instance;
}

void ResourceManager::Init()
{
	modelResources.Init(32, 8);
	modelResources.DeclareHashFunction(String::StringHash);

	textureResources.Init(64, 8);
	textureResources.DeclareHashFunction(String::StringHash);

	sceneResources.Init(32, 8);
	sceneResources.DeclareHashFunction(String::StringHash);

	shaderResources.Init(32, 8);
	shaderResources.DeclareHashFunction(String::StringHash);

	entityResources.Init(64, 8);
	entityResources.DeclareHashFunction(String::StringHash);

	vertexAnimatedResources.Init(32, 8);
	vertexAnimatedResources.DeclareHashFunction(String::StringHash);

	listResources.Init(64, 8);
	listResources.DeclareHashFunction(String::StringHash);

	settings = nullptr;
}

ResourceManager::~ResourceManager()
{
	Unload();
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

	for (UINT i = 0; i < textureResources.GetSize(); i++)
	{
		delete textureResources[i];
	}

	for (UINT i = 0; i < sceneResources.GetSize(); i++)
	{
		delete sceneResources[i];
	}

	for (UINT i = 0; i < shaderResources.GetSize(); i++)
	{
		delete shaderResources[i];
	}

	for (UINT i = 0; i < entityResources.GetSize(); i++)
	{
		delete entityResources[i];
	}
}

const EntityResource *ResourceManager::LoadEnitity(const char * path)
{
	EntityResource *entity;

	if (entityResources.Find(path, entity))
	{
		// if already loaded
		return entity;
	}

	// allocate
	entity = new EntityResource();

	using namespace tinyxml2;

	XMLDocument doc;
	doc.LoadFile(path);

	XMLElement *root = doc.RootElement();
	if (!root)
	{
		// if can't parse root
		return nullptr;
	}

	// count components
	int componentsCount = 0;
	for (XMLElement *node = root->FirstChildElement();
		node;
		node = node->NextSiblingElement())
	{
		componentsCount++;
	}

	entity->Init(componentsCount);
	entity->isActive = root->BoolAttribute(ENTITY_KEY_ACTIVE, true);
	entity->path = path;

	const char *val;

	if (val = root->Attribute("Name"))
	{
		// set name
		entity->name = val;
	}

	if (val = root->Attribute("Position"))
	{
		entity->transform.SetPosition(String::ToVector3(val));
	}

	if (val = root->Attribute("Euler"))
	{
		entity->transform.SetRotation(String::ToVector3(val));
	}

	if (val = root->Attribute("Quaternion"))
	{
		entity->transform.SetRotation(String::ToQuaternion(val));
	}

	if (val = root->Attribute("Scale"))
	{
		entity->transform.SetScale(String::ToVector3(val));
	}

	int index = 0;

	// foreach component in xml
	for (XMLElement *element = root->FirstChildElement();
		element;
		element = element->NextSiblingElement())
	{
		// count attributes in this component
		int attributeCount = 0;
		const XMLAttribute *attr = element->FirstAttribute();
		while (attr != nullptr)
		{
			attr = attr->Next();
			attributeCount++;
		}

		// allocate
		ComponentResource *component = new ComponentResource();
		component->Init(attributeCount);

		component->isActive = element->BoolAttribute(COMPONENT_KEY_ACTIVE, true);
		component->name = element->Value();
		
		int attrIndex = 0;

		attr = element->FirstAttribute();
		while (attr != nullptr)
		{
			component->tuples[attrIndex].Init(attr->Name(), attr->Value());

			// get next
			attr = attr->Next();
			attrIndex++;
		}

		entity->components[index] = component;
		index++;
	}

	entityResources.Add(path, entity);
	return entity;
}

const VertexAnimatedResource *ResourceManager::LoadVertexAnimated(const char * path)
{
	VertexAnimatedResource *vertAnim;

	if (vertexAnimatedResources.Find(path, vertAnim))
	{
		// if already loaded
		return vertAnim;
	}

	// allocate
	vertAnim = new VertexAnimatedResource();
	
	std::ifstream file;
	std::string line;

	// open
	file.open(path);

	// count lines
	int modelTimeCount = 0;

	// first must be name
	std::getline(file, line);
	vertAnim->name = line.c_str();

	// second must be ticks per second
	std::getline(file, line);
	vertAnim->ticksPerSecond = String::ToFloat(line.c_str());

	// else must be pairs model-time
	while (std::getline(file, line))
	{
		modelTimeCount++;
	}

	// reset to beginning
	file.clear();
	file.seekg(0);
	// skip animation name and ticks per second
	std::getline(file, line);
	std::getline(file, line);

	// init array
	// for each anim node there 2 lines
	int animNodeCount = modelTimeCount / 2;
	vertAnim->animationNodes.Init(animNodeCount);

	// for each line
	for (int i = 0; i < animNodeCount; i++)
	{
		// first is time
		std::getline(file, line);

		if (line.length() == 0)
		{
			continue;
		}

		vertAnim->animationNodes[i].Time = (float)atof(line.c_str());
		
		// second is a path to model
		std::getline(file, line);
		// so load model resource
		vertAnim->animationNodes[i].Value = LoadModel(line.c_str());
	}

	// close
	file.close();

	// get duration as last frame's time
	vertAnim->duration = vertAnim->animationNodes[animNodeCount - 1].Time;

	// register
	vertexAnimatedResources.Add(path, vertAnim);

	return vertAnim;
}

const ListResource *ResourceManager::LoadList(const char * path)
{
	ListResource *list;

	// if already loaded
	if (listResources.Find(path, list))
	{
		return list;
	}

	list = new ListResource(path);

	std::ifstream file;
	std::string line;

	// open
	file.open(path);

	// count lines
	int linesCount = 0;

	while (std::getline(file, line))
	{
		// dont ignore empty
		linesCount++;
	}

	// reset to beginning
	file.clear();
	file.seekg(0);

	// init array
	list->lines.Init(linesCount);

	// for each line
	for (int i = 0; i < linesCount; i++)
	{
		std::getline(file, line);
		
		// raw init, as array was initted using malloc
		list->lines[i].Init(line.c_str());
	}

	// close
	file.close();

	listResources.Add(path, list);
	return list;
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

const SceneResource *ResourceManager::LoadScene(const char * path)
{
	SceneResource *scene;

	if (sceneResources.Find(path, scene))
	{
		return scene;
	}

	// allocate
	scene = new SceneResource(path);

	using namespace tinyxml2;

	XMLDocument doc;
	doc.LoadFile(path);

	XMLElement *root = doc.RootElement();
	if (!root)
	{
		// if can't parse root
		return nullptr;
	}

	// count entities
	int entitiesCount = 0;
	for (XMLElement *element = root->FirstChildElement();
		element;
		element = element->NextSiblingElement())
	{
		if (String::Compare(element->Value(), "Entity"))
		{
			entitiesCount++;
		}
	}

	const char *val;

	if (val = root->Attribute("Name"))
	{
		// if name is found
		scene->Init(entitiesCount, val);
	}
	else
	{
		scene->Init(entitiesCount);
	}

	// entity index
	int i = 0;

	// foreach entity in xml
	for (XMLElement *element = root->FirstChildElement();
		element;
		element = element->NextSiblingElement())
	{
		if (String::Compare(element->Value(), "Skybox"))
		{
			if (val = element->Attribute("Front"))
			{
				scene->skyboxSideNames[0] = val;
			}

			if (val = element->Attribute("Back"))
			{
				scene->skyboxSideNames[1] = val;
			}

			if (val = element->Attribute("Up"))
			{
				scene->skyboxSideNames[2] = val;
			}

			if (val = element->Attribute("Down"))
			{
				scene->skyboxSideNames[3] = val;
			}

			if (val = element->Attribute("Right"))
			{
				scene->skyboxSideNames[4] = val;
			}

			if (val = element->Attribute("Left"))
			{
				scene->skyboxSideNames[5] = val;
			}
		}

		if (!String::Compare(element->Value(), "Entity"))
		{
			continue;
		}

		const XMLAttribute *attr = element->FirstAttribute();
		
		if (val = element->Attribute("Path"))
		{
			scene->entitiesData[i].EntityPath = val;
		}

		if (val = element->Attribute("Position"))
		{
			scene->entitiesData[i].Transformation.SetPosition(String::ToVector3(val));
			scene->entitiesData[i].IsTransformed = true;
		}

		if (val = element->Attribute("Euler"))
		{
			scene->entitiesData[i].Transformation.SetRotation(String::ToVector3(val));
			scene->entitiesData[i].IsTransformed = true;
		}

		if (val = element->Attribute("Quaternion"))
		{
			scene->entitiesData[i].Transformation.SetRotation(String::ToQuaternion(val));
			scene->entitiesData[i].IsTransformed = true;
		}

		if (val = element->Attribute("Scale"))
		{
			scene->entitiesData[i].Transformation.SetScale(String::ToVector3(val));
			scene->entitiesData[i].IsTransformed = true;
		}

		i++;
	}


	// register
	sceneResources.Add(path, scene);

	return scene;
}

const ShaderResource *ResourceManager::LoadShader(const char *vertexPath, const char *fragmentPath)
{
	ShaderResource *shader;

	// TODO: vertpath and fragpath tuple indexing
	if (shaderResources.Find(vertexPath, shader))
	{
		return shader;
	}

	// allocate
	shader = new ShaderResource();

	// stream
	std::ifstream vertFile, fragFile;
	
	// exceptions
	vertFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		std::string vertCode, fragCode;
		std::stringstream vertStream, fragStream;

		// open files
		vertFile.open(vertexPath);
		fragFile.open(fragmentPath);
		
		// read file's buffer contents into streams
		vertStream << vertFile.rdbuf();
		fragStream << fragFile.rdbuf();
		
		// close file handlers
		vertFile.close();
		fragFile.close();

		// convert stream into string
		// and assign to shader
		shader->vertexCode = vertStream.str().c_str();
		shader->fragmentCode = fragStream.str().c_str();

		shader->vertexPath = vertexPath;
		shader->fragmentPath = fragmentPath;
	}
	catch (const std::exception&)
	{
		Logger::Print("Shaders::Can't read shader code");
	}

	// register
	shaderResources.Add(vertexPath, shader);

	return shader;
}

#pragma region ModelLoading
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
		meshes[i] = new MeshResource(sourceMesh->mNumVertices, indexCount, sourceMesh->mNumFaces, sourceMesh->mNumBones, sourceMesh->mMaterialIndex);

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
				fromNode->mNodeName.C_Str(),
				fromNode->mNumPositionKeys, 
				fromNode->mNumRotationKeys,
				fromNode->mNumScalingKeys);
		}

		// fill data
		CopyAnimation(from, animations[i]);
	}

	// allocate
	resultModel = new ModelResource(path, hierarchy, (int)scene->mNumMaterials);

	// copy each material
	for (UINT i = 0; i < scene->mNumMaterials; i++)
	{
		CopyMaterial(scene->mMaterials[i], resultModel->materials[i]);
	}

	// add to hash table
	modelResources.Add(path, resultModel);

	return resultModel;
}

void ResourceManager::CopyMaterial(void *from, Material *to)
{
	aiMaterial *source = (aiMaterial*)from;
	StandardMaterial *target = (StandardMaterial*)to;
	
	aiColor4D diffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	aiColor4D specularColor(1.0f, 1.0f, 1.0f, 1.0f);
	aiColor4D reflectiveColor(1.0f, 1.0f, 1.0f, 1.0f);
	float shininess;
	float reflectivity;

	source->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
	source->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
	source->Get(AI_MATKEY_COLOR_REFLECTIVE, reflectiveColor);
	source->Get(AI_MATKEY_SHININESS, shininess);
	source->Get(AI_MATKEY_REFLECTIVITY, reflectivity);

	target->SetMainColor(Color4F(diffuseColor.r, diffuseColor.g, diffuseColor.b, diffuseColor.a));
	target->SetSpecularColor(Color4F(specularColor.r, specularColor.g, specularColor.b, specularColor.a));

	aiString diffuseTexturePath;

	if (source->GetTexture(aiTextureType_DIFFUSE, 0, &diffuseTexturePath) == AI_SUCCESS)
	{
		Texture *diffuseTexture = new Texture();
		diffuseTexture->Load(diffuseTexturePath.C_Str());

		target->AddTexture(diffuseTexture);
	}
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

	Skeleton *skeleton = to->skeleton;
	StaticArray<Bone> &bones = skeleton->bones;
	StaticArray<VertexWeight> &weights = skeleton->vertexWeights;

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
				// engine uses transposed transformation matrices
				m(y, x) = orig->mOffsetMatrix[x][y];
			}
		}

		// find bone node and parent bone
		const ModelNode *boneNode = hierarchy->FindNode(boneName);
		
		// index of 
		int parentBoneId = -1;

		// if not a root node
		if (boneNode->parent != nullptr)
		{
			// get bone's parent node name
			const String &parentNodeName = boneNode->parent->GetName();

			// try to find bone with the same name
			// Note: finding is not in Bone class
			//       because not all bones are set
			for (UINT j = 0; j < mesh->mNumBones; j++)
			{
				// if names are equal
				if (parentNodeName == mesh->mBones[j]->mName.C_Str())
				{
					// save index j
					parentBoneId = j;
					break;
				}
			}
		}

		// create bone
		bones[i] = Bone(i, parentBoneId, boneNode, m);
	}

	// all bones are created
	// now copy their weights
	for (UINT boneIndex = 0; boneIndex < mesh->mNumBones; boneIndex++)
	{
		aiBone *orig = mesh->mBones[boneIndex];

		// assimp stores vertex weight in a bone
		int weightsCountInBone = (int)orig->mNumWeights;

		// for each assimp weight in this bone
		for (int w = 0; w < weightsCountInBone; w++)
		{
			int vertIndex = (int)orig->mWeights[w].mVertexId;
			float weight = orig->mWeights[w].mWeight;

			// add
			weights[vertIndex].AddWeight((int)boneIndex, weight);
		}
	}

	// all weights are setted
	// norrmalize them to make
	// their weight sum equal to 1.0f
	UINT weightsCount = skeleton->vertexWeights.GetSize();
	for (UINT i = 0; i < weightsCount; i++)
	{
		weights[i].Normalize();
	}

	// all bones are set
	// init skeleton
	skeleton->Init();
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
	
	//// if root node, inverse its transformation
	//if (parentNode == nullptr)
	//{
	//	transformation.Inverse();
	//}

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
#pragma endregion

const GlobalSettings *ResourceManager::LoadSettings(const char * path)
{
	ASSERT(settings == nullptr);

	// allocate
	settings = new GlobalSettings();

	using namespace tinyxml2;

	XMLDocument doc;
	doc.LoadFile(path);

	XMLElement *root = doc.RootElement();
	if (!root)
	{
		// if can't parse root
		return nullptr;
	}

	const char *val;

	if (val = root->Attribute("Name"))
	{
		settings->Name = val;
	}

	if (val = root->Attribute("ScreenWidth"))
	{
		settings->ScreenWidth = String::ToInt(val);
	}

	if (val = root->Attribute("ScreenHeight"))
	{
		settings->ScreenHeight = String::ToInt(val);
	}

	if (val = root->Attribute("StartScenePath"))
	{
		settings->StartScenePath = val;
	}

	if (val = root->Attribute("StandardShaderVert"))
	{
		settings->StandardShaderVert = val;
	}

	if (val = root->Attribute("StandardShaderFrag"))
	{
		settings->StandardShaderFrag = val;
	}

	if (val = root->Attribute("SkyboxShaderVert"))
	{
		settings->SkyboxShaderVert = val;
	}

	if (val = root->Attribute("SkyboxShaderFrag"))
	{
		settings->SkyboxShaderFrag = val;
	}

	if (val = root->Attribute("DebugShaderVert"))
	{
		settings->DebugShaderVert = val;
	}

	if (val = root->Attribute("DebugShaderFrag"))
	{
		settings->DebugShaderFrag = val;
	}

	return settings;
}