#pragma once

#include <Engine/Components/IComponent.h>
#include <Engine/Math/Transform.h>
#include <Engine/DataStructures/DynamicArray.h>
#include <vector>

class Entity
{
	friend class EntityFactory;

private:
	EntityID entityId;

	bool isActive;
	int layer;
	int tag;

	DynamicArray<IComponent*> components;

private:
	Entity(EntityID id);
	Entity(const Entity&) = delete;
	Entity(const Entity&&) = delete;

	// Loads main data from xml element
	bool PreInit(void *root);
	// Init after loading components
	void Init();

// used by factory
private:
	void AddComponent(IComponent *c);

protected:
	Transform transform;

public:
	template <class T>
	T *GetComponent();
	template <class T>
	const T *GetComponent() const;
	const DynamicArray<IComponent*> &GetAllComponents() const;

	void Destroy();

public:
	void SetActive(bool active);
	bool IsActive();
	EntityID GetID() const;

	// Returns reference
	Transform &GetTransform();
	// Returns copy of const Entity
	const Transform &GetTransform() const;
};

template<class T>
const T *Entity::GetComponent() const
{
	return GetComponent();
}

template<class T>
T *Entity::GetComponent()
{
	for (int i = 0; i < components.GetSize(); i++)
	{
		IComponent *c = components[i];

		if (c->IsClassType(T::Type))
		{
			return (T*)c;
		}
	}

	return nullptr;
}
