#pragma once

#include <Engine/Components/IComponent.h>
#include <Engine/Math/Transform.h>
#include <Engine/DataStructures/DynamicArray.h>
#include <vector>

class Entity
{
	friend class EntityFactory;
	//temp
	friend class Rigidbody;

private:
	// Unique entity ID
	EntityID entityId;

	// Is entity active?
	bool isActive;
	int layer;
	int tag;

	// All components attached to this entity
	// Contains only refernces, not the components themselves
	// They are allocated in EntityFactory
	DynamicArray<IComponent*> components;

	// Name of this enitity, must not be changed
	String name;

protected:
	// Current transformation of the entity
	Transform transform;

private:
	Entity(EntityID id);
	Entity(const Entity&) = delete;
	Entity(const Entity&&) = delete;

	// Init before loading components
	void Init();
	// Destroys all attached components 
	void Destroy();

	// used by factory
	void AddComponent(IComponent *c);

public:
	// Get component by its type
	template <class T>
	T *GetComponent();
	// Get component by its type
	template <class T>
	const T *GetComponent() const;

	// Get all attached components to this entity
	const DynamicArray<IComponent*> &GetAllComponents() const;

public:
	// Destroys all attached components 
	~Entity();
	
	// Get unique ID of this entity
	EntityID GetID() const;
	// Get name of this entity
	const String &GetName() const;

	// Activate this entity
	void SetActive(bool active);
	// Is this entity active?
	bool IsActive() const;

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

		// check its type
		if (c->IsClassType(T::Type))
		{
			// return
			return (T*)c;
		}
	}

	// component is not found
	return nullptr;
}
