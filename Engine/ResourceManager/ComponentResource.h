#pragma once
#include <Engine/DataStructures/StaticArray.h>
#include <Engine/Base/StringTuple.h>

class ComponentResource
{
	friend class ResourceManager;

private:
	StaticArray<StringTuple> tuples;

	String	name;
	bool	isActive;

public:
	// Allocates memory for tuples
	ComponentResource();
	// Destroys tuples
	~ComponentResource();

	void Init(int tupleCount);

	// Get keys and values in this component
	const StaticArray<StringTuple> &GetKeysValues() const;

	// Get name of this entity
	const String	&GetName() const;
	// Is this component active
	bool			IsActive() const;
};
