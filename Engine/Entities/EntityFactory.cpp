#include "EntityFactory.h"
#include <Engine/TEMP/tinyxml2/tinyxml2.h>
#include <Engine/Components/CCamera.h>
#include <Engine/Components/CFreeMovement.h>
#include <Engine/Components/CModel.h>
#include <Engine/Components/CSkinnedModel.h>
#include <Engine/Components/CLight.h>
#include <Engine/Components/CParticleSystem.h>
#include <Engine/Physics/Rigidbody.h>
#include <Engine/Systems/ComponentSystem.h>

template <class T>
IComponent *CompCreator(void *elemPointer);

template<class T>
IComponent *CompCreator(void *elemPointer)
{
	using namespace tinyxml2;
	XMLElement *elem = (XMLElement*)elemPointer;

	IComponent *c = new T();

	const XMLAttribute *attr = elem->FirstAttribute();
	while (attr != nullptr)
	{
		c->SetProperty(String(attr->Name()), String(attr->Value()));
		attr = attr->Next();
	}

	return c;
}

EntityID EntityFactory::GetNextEntityID()
{
	lastId++;
	return lastId;
}

EntityFactory::EntityFactory()
{
	// init dynamic array
	entities.Init(128);
	// register in component system
	ComponentSystem::Instance().Register(&entities);
	
	// init hash table
	compCreators.Init(10, 6);
	compCreators.DeclareHashFunction(String::StringHash);

	// Builtin components
	PRegisterComponentType<CCamera>("CCamera");
	PRegisterComponentType<CFreeMovement>("CFreeMovement");
	PRegisterComponentType<CLight>("CLight");
	PRegisterComponentType<CModel>("CModel");
	PRegisterComponentType<CSkinnedModel>("CSkinnedModel");
	PRegisterComponentType<CParticleSystem>("CParticleSystem");
	PRegisterComponentType<Rigidbody>("Rigidbody");
}

EntityFactory::~EntityFactory()
{
	for (int i = 0; i < entities.GetSize(); i++)
	{
		auto &components = entities[i]->GetAllComponents();

		// delete each component in entity
		for (int i = 0; i < components.GetSize(); i++)
		{
			delete components[i];
		}

		// delete entity itself
		delete entities[i];
	}
}

IComponent *EntityFactory::CreateComponent(void *xmlElemP)
{
	using namespace tinyxml2;
	XMLElement *xmlElem = (XMLElement*)xmlElemP;

	// get component name
	const char *compName = xmlElem->Value();

	IComponent *comp;

	IComponentCreator foundCreator;
	if (compCreators.Find(compName, foundCreator))
	{
		// IComponentCreator creator = found;
		comp = foundCreator(xmlElem);
	}
	else
	{
		// if can't find component creator
		return nullptr;
	}

	return comp;
}

void EntityFactory::SetData(Entity *entity, void *xmlElem)
{
	using namespace tinyxml2;
	XMLElement *elem = (XMLElement*)xmlElem;

	entity->isActive = elem->BoolAttribute("Active", true);

	if (const char *val = elem->Attribute("Position"))
	{
		entity->transform.SetPosition(String::ToVector3(val));
	}

	if (const char *val = elem->Attribute("Euler"))
	{
		entity->transform.SetRotation(String::ToVector3(val));
	}

	if (const char *val = elem->Attribute("Quaternion"))
	{
		entity->transform.SetRotation(String::ToQuaternion(val));
	}

	if (const char *val = elem->Attribute("Scale"))
	{
		entity->transform.SetScale(String::ToVector3(val));
	}
}

void EntityFactory::SetData(Entity *entity, const Entity *source)
{
	entity->isActive = source->isActive;
	entity->transform = source->transform;
}

Entity *EntityFactory::PCreateEntity(const char *resource)
{
	using namespace tinyxml2;

	XMLDocument doc;
	doc.LoadFile(resource);

	XMLElement *root = doc.RootElement();
	if (!root)
	{
		// if can't parse root
		return nullptr;
	}

	Entity *entity = new Entity(GetNextEntityID());
	entity->Init();

	// load main data
	SetData(entity, root);

	// foreach component in xml
	for (XMLElement *node = root->FirstChildElement();
		node;
		node = node->NextSiblingElement())
	{
		IComponent *comp = CreateComponent(node);

		// if component created
		ASSERT(comp != nullptr);

		if (node->BoolAttribute("Active", true))
		{
			// must be activated through the field
			// else will be activated as a component
			comp->isActive = true;
		}

		// link
		entity->AddComponent(comp);
		comp->SetOwner(entity);

		// everything is set up
		comp->Init();
	}
	
	// finally, store entity
	entities.Push(entity);

	return entity;
}

Entity * EntityFactory::PCreateEntity(const Entity *source)
{
	Entity *entity = new Entity(GetNextEntityID());
	entity->Init();

	// load main data
	SetData(entity, source);

	auto &components = source->GetAllComponents();

	// foreach component in source
	for (int i = 0; i < components.GetSize(); i++)
	{
		ASSERT(0);
	}

	// finally, store entity
	entities.Push(entity);

	return entity;
}

EntityFactory &EntityFactory::Instance()
{
	static EntityFactory instance;
	return instance;
}

Entity *EntityFactory::CreateEntity(const char *resource)
{
	return EntityFactory::Instance().PCreateEntity(resource);
}

Entity *EntityFactory::CreateEntity(const Entity *entity)
{
	return EntityFactory::Instance().PCreateEntity(entity);
}
