#include "EntityFactory.h"
#include <Engine/TEMP/tinyxml2/tinyxml2.h>
#include <Engine/Components/CCamera.h>
#include <Engine/Components/CFreeMovement.h>
#include <Engine/Components/CModel.h>
#include <Engine/Components/CLight.h>
#include <Engine/Base/String.h>

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
	compCreators.Init(10, 6);
	compCreators.DeclareHashFunction(String::StringHash);

	compCreators.Add("CCamera", CompCreator<CCamera>);
	compCreators.Add("CFreeMovement", CompCreator<CFreeMovement>);
	compCreators.Add("CLight", CompCreator<CLight>);
	compCreators.Add("CModel", CompCreator<CModel>);
}

IComponent *EntityFactory::CreateComponent(void *xmlElemP)
{
	using namespace tinyxml2;
	XMLElement *xmlElem = (XMLElement*)xmlElemP;

	// get component name
	const char *compName = xmlElem->Value();

	IComponent *comp;

	// auto found = componentCreators.find(compName);
	// if (found != componentCreators.end())
	
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

	// load main data
	if (!entity->PreXMLInit(root))
	{
		delete entity;
		// fail
		return nullptr;
	}

	// foreach component in xml
	for (XMLElement *node = root->FirstChildElement();
		node;
		node = node->NextSiblingElement())
	{
		IComponent *comp = CreateComponent(node);
		
		// if component created
		if (comp != nullptr)
		{
			if (node->BoolAttribute("active", true))
			{
				// must be activated through field
				// else will be activated as a component
				comp->isActive = true;
			}
			
			// link
			entity->AddComponent(comp);
			comp->SetOwner(entity);

			// all is set up
			comp->Init();
		}
		else
		{
			delete entity;

			// if some node incorrect
			return nullptr;
		}
	}

	entity->Init();
	return entity;
}

EntityFactory &EntityFactory::Instance()
{
	static EntityFactory instance;
	return instance;
}

Entity *EntityFactory::CreateEntity(const char * resource)
{
	return EntityFactory::Instance().PCreateEntity(resource);
}
