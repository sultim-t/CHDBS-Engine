//#pragma once
//
//#include "EntityRenderer.h"
//#include "Camera.h"
//#include "Model.h"
//
//#include <map>
//#include <list>
//
//class Renderer
//{
//private:	
//	Renderer();
//	~Renderer();
//
//	Renderer(Renderer const&) = delete;
//	Renderer& operator= (Renderer const&) = delete;
//
//private:
//	std::map<Model, std::list<Entity>> entitiesMap;
//
//public:
//	static Renderer &Instance();
//
//	void AddEntity(Entity &entity);
//	void Render(const Camera &camera, const StaticShader &shader);
//};