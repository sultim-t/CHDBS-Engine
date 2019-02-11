//#pragma once
//
//#include "StaticShader.h"
//
//#include <map>
//#include <list>
//
//class EntityRenderer
//{
//private:
//	EntityRenderer();
//	~EntityRenderer();
//
//	EntityRenderer(EntityRenderer const&) = delete;
//	EntityRenderer& operator= (EntityRenderer const&) = delete;
//
//private:
//	void BindModel(const Model &model);
//	void UnbindModel(const Model &model);
//	void PrepareEntity(const Entity &entity, const StaticShader &shader);
//	void DrawModel(const Model &model);
//
//public:
//	static EntityRenderer &Instance()
//	{
//		static EntityRenderer instance;
//		return instance;
//	}
//
//	void Clear();
//	void Render(const std::map<Model, std::list<Entity>> &entitiesMap, const StaticShader &shader);
//	// void Render(const Entity &entity, const StaticShader &shader);
//};