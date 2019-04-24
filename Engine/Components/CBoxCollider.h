#pragma once
#include <Engine/Components/IComponent.h>
#include <Engine/Physics/AABBCollider.h>

class CBoxCollider : public AABBCollider, public IComponent
{
	CLASSDECLARATION(CBoxCollider)

public:
	void Init() override;
	void Update() override;

	void SetProperty(const String &key, const String &value) override;

public:
	virtual const Transform &GetTransform() const override;
};