#pragma once
#include <Engine/DataStructures/DynamicArray.h>
#include <Engine/Base/StringTuple.h>

class ComponentResource
{
	friend class ResourceManager;

private:
	DynamicArray<StringTuple> tuples;

	String	name;
	bool	isActive;

public:
	// Allocates memory for tuples
	ComponentResource();
	// Destroys tuples
	~ComponentResource();

	void Init();

	// Get keys and values in this component
	const DynamicArray<StringTuple> &GetKeysValues() const;

	// Get name of this entity
	const String	&GetName() const;
	// Is this component active
	bool			IsActive() const;
};
