#pragma once
#include <Engine/Components/IComponent.h>
#include <Engine/Physics/SphereCollider.h>

class CSphereCollider : SphereCollider, public IComponent
{
	CLASSDECLARATION(CSphereCollider)

private:

public:
	void Init() override;
	void Update() override;

	void SetProperty(const String &key, const String &value) override;
};