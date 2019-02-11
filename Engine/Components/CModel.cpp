#include "CModel.h"
#include <Engine/Math/Vector.h>
#include <Engine/Rendering/Texture.h>
#include <Engine/Components/CMesh.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

CLASSDEFINITION(IComponent, CModel)

bool CModel::operator<(const CModel & model) const
{
	return modelId < model.modelId;
}

void CModel::AddMesh(const Mesh & mesh)
{
	meshes.push_back(mesh);
}

void CModel::AddMeshTexture(const UINT index, const Texture & texture)
{
	if (index < meshes.size())
	{
		meshes[index].Textures.push_back(texture);
	}
}

void CModel::Draw(Shader & shader)
{
	if (!isActive)
	{
		return;
	}

	for (unsigned i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(shader);
	}
}

const std::vector<Mesh>& CModel::GetMeshes() const
{
	return meshes;
}

void CModel::Init()
{ }

void CModel::Update()
{ }

void CModel::Load(const char * path)
{
	using namespace std;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	// check for errors
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		printf(importer.GetErrorString());
		return;
	}

	// retrieve the directory path of the filepath
	string p = string(path);
	directory = p.substr(0, p.find_last_of('/'));

	// process ASSIMP's root node recursively
	ProcessNode(scene->mRootNode, scene);
}

// processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
void CModel::ProcessNode(void *n, const void *s)
{
	aiNode *node = (aiNode*)n;
	aiScene *scene = (aiScene*)s;

	// process each mesh located at the current node
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		// the node object only contains indices to index the actual objects in the scene. 
		// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene));
	}
	// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh CModel::ProcessMesh(void *m, const void *s)
{
	using namespace std;

	aiMesh *mesh = (aiMesh*)m;
	aiScene *scene = (aiScene*)s;

	// data to fill
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	// Walk through each of the mesh's vertices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		Vector3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
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
		// texture coordinates
		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			Vector2 vec;
			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec[0] = mesh->mTextureCoords[0][i].x;
			vec[1] = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = Vector2(0.0f, 0.0f);

		// tangent
		vector[0] = mesh->mTangents[i].x;
		vector[1] = mesh->mTangents[i].y;
		vector[2] = mesh->mTangents[i].z;
		//!!!!!!!!!!!!!
		//vertex.Tangent = vector;

		// bitangent
		vector[0] = mesh->mBitangents[i].x;
		vector[1] = mesh->mBitangents[i].y;
		vector[2] = mesh->mBitangents[i].z;
		//vertex.Bitangent = vector;

		vertices.push_back(vertex);
	}
	// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	// process materials
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
	// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
	// Same applies to other texture as the following list summarizes:
	// diffuse: texture_diffuseN
	// specular: texture_specularN
	// normal: texture_normalN

	// 1. diffuse maps
	vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, TEXTURE_DIFFUSE);
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	// 2. specular maps
	vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, TEXTURE_SPECULAR);
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	// 3. normal maps
	std::vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, TEXTURE_NORMAL);
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	// 4. height maps
	std::vector<Texture> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, TEXTURE_HEIGHT);
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	// return a mesh object created from the extracted mesh data
	return Mesh(vertices, indices, textures);
}


// checks all material textures of a given type and loads the textures if they're not loaded yet.
// the required info is returned as a Texture struct.
std::vector<Texture> CModel::LoadMaterialTextures(void *m, int t, TextureType myType)
{
	using namespace std;

	aiMaterial *mat = (aiMaterial*)m;
	aiTextureType type = (aiTextureType)t;

	vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (unsigned int j = 0; j < textures.size(); j++)
		{
			if (std::strcmp(loadedTextures[j].GetPath(), str.C_Str()) == 0)
			{
				textures.push_back(loadedTextures[j]);
				skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)
		{
			// if texture hasn't been loaded already, load it
			string s = this->directory + '/' + str.C_Str();

			Texture texture = Texture();
			texture.Load(s.c_str());
			texture.SetType(myType);

			textures.push_back(texture);
			loadedTextures.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}
	return textures;
}