#include "EntityFactory.h"
#include <Engine/TEMP/tinyxml2/tinyxml2.h>
#include <Engine/Components/CCamera.h>
#include <Engine/Components/CFreeMovement.h>
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
	// componentCreators.insert(std::pair<std::string, IComponentCreator>("CCamera", CompCreator<CCamera>));
	// componentCreators.insert(std::pair<std::string, IComponentCreator>("CFreeMovement", CompCreator<CFreeMovement>));

	compCreators.Init(10, 6);
	compCreators.DeclareHashFunction(String::StringHash);

	compCreators.Add("CCamera", CompCreator<CCamera>);
	compCreators.Add("CFreeMovement", CompCreator<CFreeMovement>);
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

	if (comp)
	{
		comp->Init();
	}

	return comp;
}

Entity *EntityFactory::CreateEntity(const char *resource)
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

	// TEMP: must be allocated in pool
	Entity *entity = new Entity(GetNextEntityID());

	// load main data
	if (!entity->PreXMLInit(root))
	{
		// fail
		return nullptr;
	}

	// foreach component in xml
	for (XMLElement *node = root->FirstChildElement();
		node;
		node = node->NextSiblingElement())
	{
		IComponent *comp = CreateComponent(node);
		if (comp)
		{
			entity->AddComponent(comp);
			comp->SetOwner(entity);
		}
		else
		{
			// if some node incorrect
			return nullptr;
		}
	}

	entity->Init();
	return entity;
}

EntityFactory & EntityFactory::Instance()
{
	static EntityFactory instance;
	return instance;
}
