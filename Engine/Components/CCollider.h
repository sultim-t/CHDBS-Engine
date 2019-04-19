#pragma once
#include <Engine/Physics/ICollider.h>

class CCollider
{
private:
	// Function to identify material combine option
	static PhysicMaterialCombine GetCombineOption(const String &value);

public:
	// Set property to collider
	static bool SetProperty(ICollider *collider, const String &key, const String &value);
};