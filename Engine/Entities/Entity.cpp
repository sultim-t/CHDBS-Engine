#include "Entity.h"
#include <Engine\TEMP\tinyxml2\tinyxml2.h>

void Entity::AddComponent(IComponent * c)
{
	components->push_back(c);
}

void Entity::RemoveComponent(IComponent * c)
{
}

Entity::Entity(EntityID id)
{
	components = new std::vector<IComponent*>();

	entityId = id;
	isActive = true;
}

bool Entity::PreXMLInit(void * root)
{
	using namespace tinyxml2;

	XMLElement *elem = (XMLElement*)root;

	Vector3 pos, scale;

	pos[0] = elem->FloatAttribute("posx");
	pos[1] = elem->FloatAttribute("posy");
	pos[2] = elem->FloatAttribute("posz");

	if (elem->FindAttribute("qw"))
	{
		Quaternion q;
		q[0] = elem->FloatAttribute("qw", 1);
		q[1] = elem->FloatAttribute("qx", 0);
		q[2] = elem->FloatAttribute("qy", 0);
		q[2] = elem->FloatAttribute("qz", 0);

		transform.SetRotation(q);
	}
	else if (elem->FindAttribute("eulx"))
	{
		Euler e;
		e[0] = elem->FloatAttribute("eulx");
		e[1] = elem->FloatAttribute("euly");
		e[2] = elem->FloatAttribute("eulz");

		transform.SetRotation(e);
	}
	else
	{
		transform.Reset();
	}

	scale[0] = elem->FloatAttribute("scalex", 1);
	scale[1] = elem->FloatAttribute("scaley", 1);
	scale[2] = elem->FloatAttribute("scalez", 1);

	transform.SetPosition(pos);
	transform.SetScale(pos);

	return true;
}

void Entity::Init()
{
}

Entity::Entity()
{
	// TEMP
	components = new std::vector<IComponent*>();
	isActive = true;
}

void Entity::Destroy()
{
}

void Entity::SetActive(bool active)
{
	isActive = true;
}

bool Entity::IsActive()
{
	return isActive;
}

bool Entity::IsActiveInHierarchy()
{
	return true;
}

EntityID Entity::GetID() const
{
	return entityId;
}

Transform &Entity::GetTransform()
{
	return transform;
}

const Transform Entity::GetTransform() const
{
	return transform;
}
