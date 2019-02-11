//#include "EntityRenderer.h"
//#include "OpenGL.h"
//#include <Engine\Math\Projection.h>
//#include <Engine\Rendering\Model.h>
//#include <Engine\Entities\Entity.h>
//
//EntityRenderer::EntityRenderer()
//{
//	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_CULL_FACE);
//	glCullFace(GL_BACK);
//}
//
//EntityRenderer::~EntityRenderer()
//{}
//
//void EntityRenderer::Clear()
//{
//	glClearColor(0.5f, 0.5f, 0.75f, 1.0f);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//}
//
//void EntityRenderer::BindModel(const Model & model)
//{
//	for (Mesh mesh : model.GetMeshes())
//	{
//		glBindVertexArray(mesh.GetVAO());
//		glEnableVertexAttribArray(0);
//		glEnableVertexAttribArray(1);
//		glEnableVertexAttribArray(2);
//
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, mesh.Textures[0].GetID());
//	}
//}
//
//void EntityRenderer::UnbindModel(const Model & model)
//{
//	glDisableVertexAttribArray(0);
//	glDisableVertexAttribArray(1);
//	glDisableVertexAttribArray(2);
//	glBindVertexArray(0);
//}
//
//void EntityRenderer::PrepareEntity(const Entity & entity, const StaticShader &shader)
//{
//	Matrix4 modelMatrix = entity.GetTransform().GetTransformMatrix();
//	shader.LoadModelMatrix(modelMatrix);
//}
//
//void EntityRenderer::DrawModel(const Model & model)
//{
//	for (Mesh mesh : model.GetMeshes())
//	{
//		//glDrawElements(GL_TRIANGLES, mesh.GetVertexCount(), GL_UNSIGNED_INT, 0);
//		glDrawArrays(GL_TRIANGLES, 0, 36);
//	}
//}
//
//void EntityRenderer::Render(const std::map<Model, std::list<Entity>>& entitiesMap, const StaticShader &shader)
//{
//	using namespace std;
//
//	for (auto const &elem : entitiesMap)
//	{
//		Model model = elem.first;
//		BindModel(model);
//
//		list<Entity> batch = entitiesMap.at(model);
//		for (Entity entity : batch)
//		{
//			// prepare transformations of each entity
//			PrepareEntity(entity, shader);
//			// draw same model with another transformations
//			DrawModel(model);
//		}
//
//		UnbindModel(model);
//	}
//}
