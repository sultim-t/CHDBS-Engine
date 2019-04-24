#pragma once
#include <Engine/Components/IComponent.h>
#include <Engine/Physics/SphereCollider.h>

class CSphereCollider : public SphereCollider, public IComponent
{
	CLASSDECLARATION(CSphereCollider)

public:
	void Init() override;
	void Update() override;

	void SetProperty(const String &key, const String &value) override;

public:
	virtual const Transform &GetTransform() const override;
};