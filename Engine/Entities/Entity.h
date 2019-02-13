#pragma once

#include <Engine/Components/IComponent.h>
#include <Engine/Math/Transform.h>
#include <Engine/DataStructures/StaticArray.h>
#include <vector>

class Entity
{
	friend class EntityFactory;

private:
	EntityID entityId;

	bool isActive;
	int layer;
	int tag;

	std::vector<IComponent*> *components;

public:
	Entity(EntityID id);
	Entity(const Entity&); // = delete;
	Entity(const Entity&&); // = delete;

	// Loads main data from xml element
	bool PreXMLInit(void *root);
	// Init after loading components
	void Init();

// temp public functions
//must be used ONLY by factory
public:
	Entity();
	void AddComponent(IComponent *c);
	void RemoveComponent(IComponent *c);

protected:
	Transform transform;

public:
	template <class T>
	T *GetComponent();
	template <class T>
	const T *GetComponent() const;
	const std::vector<IComponent*> &GetAllComponents() const;

	void Destroy();

public:
	void SetActive(bool active);
	bool IsActive();
	bool IsActiveInHierarchy();
	EntityID GetID() const;

	// Returns reference
	Transform &GetTransform();
	// Returns copy of const Entity
	const Transform &GetTransform() const;
};

template<class T>
const T * Entity::GetComponent() const
{
	return GetComponent();
}

template<class T>
T * Entity::GetComponent()
{
	auto iter = components->begin();
	auto last = components->end();

	while (iter != last)
	{
		IComponent *r = *(iter._Ptr);

		if (r->IsClassType(T::Type))
		{
			return (T*)r;
		}

		iter++;
	}

	return NULL;
}
