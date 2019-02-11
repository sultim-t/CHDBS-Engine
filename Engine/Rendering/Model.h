//#pragma once
//
//#include "Mesh.h"
//
//class Model
//{
//private:
//	std::vector<Mesh> meshes;
//	std::vector<Texture> TexturesLoaded;
//
//	std::string directory;
//	unsigned id;
//	
//	static unsigned lastId;
//
//private:
//	void ProcessNode(void *node, const void *scene);
//	Mesh ProcessMesh(void *mesh, const void *scene);
//	std::vector<Texture> LoadMaterialTextures(void *mat, int type, TextureType myType);
//
//public:
//	Model();
//	~Model();
//
//	bool operator<(const Model &model) const;
//
//	bool LoadObjFile(const char *path);
//	// Load through assimp
//	void Load(const char *path);
//
//	void AddMesh(const Mesh &mesh);
//	void AddMeshTexture(const unsigned index, const Texture &texture);
//
//	void Draw(Shader &shader);
//
//	const std::vector<Mesh> &GetMeshes() const;
//};