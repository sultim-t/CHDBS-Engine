//#include "Renderer.h"
//#include "ContextWindow.h"
//#include <Engine\Entities\Entity.h>
//#include <Engine\Math\Projection.h>
//
//Renderer::Renderer()
//{
//}
//
//Renderer::~Renderer()
//{
//}
//
//Renderer & Renderer::Instance()
//{
//	static Renderer instance;
//	return instance;
//}
//
//void Renderer::AddEntity(Entity & entity)
//{
//	using namespace std;
//
//	Model &model = entity.model;
//
//	auto foundPair = entitiesMap.find(model);
//
//	if (foundPair != entitiesMap.end())
//	{
//		list<Entity> &batch = foundPair->second;
//		batch.push_back(entity);
//	}
//	else
//	{
//		list<Entity> &newBatch = list<Entity>({ entity });
//		entitiesMap.insert(pair<Model, list<Entity>>(model, newBatch));
//	}
//}
//
//void Renderer::Render(const Camera &camera, const StaticShader &shader)
//{
//	EntityRenderer &renderer = EntityRenderer::Instance();
//
//	renderer.Clear();
//
//	shader.Use();
//
//	shader.LoadViewMatrix(camera.GetViewMatrix());
//
//	ContextWindow &window = ContextWindow::Instance();
//	float aspect = (float)window.GetWidth() / (float)window.GetHeight();
//	shader.LoadProjectionMatrix(Projection::Perspective(
//		camera.GetFOV(), 
//		aspect, 
//		camera.GetNearClipDist(),
//		camera.GetFarClipDist()));
//
//	renderer.Render(entitiesMap, shader);
//
//	shader.Stop();
//}
