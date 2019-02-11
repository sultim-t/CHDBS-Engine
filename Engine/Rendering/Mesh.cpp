//#include "Mesh.h"
//#include "OpenGL.h"
//#include <vector>
//
//void Mesh::Init()
//{	
//	// init buffers
//	glGenVertexArrays(1, &vao);
//	glGenBuffers(1, &vbo);
//	glGenBuffers(1, &ibo);
//
//	// bind
//	glBindVertexArray(vao);
//
//	glBindBuffer(GL_ARRAY_BUFFER, vbo);
//	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), Vertices.data(), GL_STATIC_DRAW);
//
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned), Indices.data(), GL_STATIC_DRAW);
//
//	// positions
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
//
//	// normals
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
//
//	// texture coords
//	glEnableVertexAttribArray(2);
//	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
//	/*
//	// vertex tangent
//	glEnableVertexAttribArray(3);
//	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
//	
//	// vertex bitangent
//	glEnableVertexAttribArray(4);
//	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
//	*/
//	glBindVertexArray(0);
//}
//
//Mesh::Mesh() {}
//
//Mesh::Mesh(const std::vector<Vertex> vertices, const std::vector<unsigned int> indices, const std::vector<Texture>textures)
//{
//	Vertices = vertices;
//	Indices = indices;
//	Textures = textures;
//}
//
//Mesh::Mesh(const std::vector<Vertex> vertices, const std::vector<Texture> textures)
//{
//	Vertices = std::vector<Vertex>();
//	Indices = std::vector<unsigned>();
//	Textures = textures;
//
//	for (unsigned i = 0; i < vertices.size(); i++)
//	{
//		unsigned index;
//		
//		// try to find similar vert in 
//		if (FindSimilarVertex(vertices[i], Vertices, index))
//		{
//			// use index of similar vertex
//			Indices.push_back(index);
//		}
//		else
//		{
//			Vertices.push_back(vertices[i]);
//			
//			// new is last in Vertices
//			unsigned newIndex = (unsigned)Vertices.size() - 1;
//			Indices.push_back(newIndex);
//		}
//	}
//}
//
//Mesh::~Mesh()
//{
//	glDeleteVertexArrays(1, &vao);
//	glDeleteBuffers(1, &vbo);
//	glDeleteBuffers(1, &ibo);
//}
//
//void Mesh::DrawElements(const Shader & shader) const
//{
//	for (unsigned i = 0; i < Textures.size(); i++)
//	{
//		Textures[i].Activate(i);
//
//		/*if (Textures[i].GetType() == TEXTURE_DIFFUSE)
//		{
//
//		}*/
//	}
//
//	glBindVertexArray(vao);
//
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
//	glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
//	
//	// to default
//	glBindVertexArray(0);
//	glActiveTexture(GL_TEXTURE0);
//}
//
//void Mesh::DrawToShadowMap(FramebufferTexture & shadowMap)
//{
//	glBindVertexArray(vao);
//	glDrawArrays(GL_TRIANGLES, 0, GetVertexCount());
//}
//
//void Mesh::AddTexture(const Texture & texture)
//{
//	Textures.push_back(texture);
//}
//
//void Mesh::Draw(const Shader & shader) const
//{
//	shader.SetMat4("model", )
//
//	for (unsigned i = 0; i < Textures.size(); i++)
//	{
//		Textures[i].Activate(i);
//
//		/*if (Textures[i].GetType() == TEXTURE_DIFFUSE)
//		{
//
//		}*/
//	}
//
//	// draw
//	glBindVertexArray(vao);
//	glDrawArrays(GL_TRIANGLES, 0, GetVertexCount());
//
//	// to default
//	glBindVertexArray(0);
//	glActiveTexture(GL_TEXTURE0);
//}
//
//bool Mesh::FindSimilarVertex(const Vertex &vert, const std::vector<Vertex> &vertices, unsigned &foundIndex)
//{
//	for (unsigned i = 0; i < vertices.size(); i++)
//	{
//		if (/*vertices[i].Normal == vert.Normal
//			&&*/ vertices[i].Position == vert.Position
//			/*&& vertices[i].TexCoords == vert.TexCoords*/)
//		{
//			foundIndex = i;
//			return true;
//		}
//	}
//
//	return false;
//}
//
//const unsigned & Mesh::GetVAO() const
//{
//	return vao;
//}
//
//unsigned Mesh::GetVertexCount() const
//{
//	return Vertices.size();
//}
//
//
//// TEMP
//void Mesh::Init(float *all, int count)
//{
//	Vertices = std::vector<Vertex>();
//	Indices = std::vector<unsigned>();
//	Textures = std::vector<Texture>();
//
//	const int varCount = 8;
//	int vertCount = count / sizeof(float) / varCount;
//
//	for (int k = 0; k < vertCount; k++)
//	{
//		Vertex v;
//		int i = k * varCount;
//
//		v.Position = Vector3(all[i], all[i + 1], all[i + 2]);
//		v.Normal = Vector3(all[i + 3], all[i + 4], all[i + 5]);
//		v.TexCoords = Vector2(all[i + 6], all[i + 7]);
//
//		Vertices.push_back(v);
//	}
//
//	// manual init
//	glGenVertexArrays(1, &vao);
//	glGenBuffers(1, &vbo);
//
//	glBindVertexArray(vao);
//
//	glBindBuffer(GL_ARRAY_BUFFER, vbo);
//	glBufferData(GL_ARRAY_BUFFER, count, all, GL_STATIC_DRAW);
//
//	// position attribute
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
//	// normals attribute
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
//	// texture coord attribute
//	glEnableVertexAttribArray(2);
//	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
//
//	glBindVertexArray(0);
//}
//
//
//
//
//
//
//
//
//
///*			unsigned diffIndex = 0;
//	unsigned specIndex = 0;
//char name[] = "texture_??";
//
//		TextureType type = Textures[i].GetType();
//		switch (type)
//		{
//		case TEXTURE_DIFFUSE:
//			name[8] = 'd';
//			name[9] = diffIndex + '0';
//			diffIndex++;
//			break;
//
//		case TEXTURE_SPECULAR:
//			name[8] = 's';
//			name[9] = specIndex + '0';
//			specIndex++;
//			break;
//		}
//
//		char test[] = "ourTexture";
//		*/