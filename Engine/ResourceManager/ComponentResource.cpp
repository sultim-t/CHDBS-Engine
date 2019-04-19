#include "ComponentResource.h"

ComponentResource::ComponentResource()
{ }

ComponentResource::~ComponentResource()
{
	// delete each tuple
	for (UINT i = 0; i < tuples.GetSize(); i++)
	{
		tuples[i].Delete();
	}
}

void ComponentResource::Init(int tupleCount)
{
	tuples.Init(tupleCount);
}

const StaticArray<StringTuple> &ComponentResource::GetKeysValues() const
{
	return tuples;
}

const String &ComponentResource::GetName() const
{
	return name;
}

bool ComponentResource::IsActive() const
{
	return isActive;
}
