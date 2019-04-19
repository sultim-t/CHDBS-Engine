#pragma once
#include <Engine/Components/IComponent.h>
#include <Engine/Physics/MeshCollider.h>

class CMeshCollider : public MeshCollider, public IComponent
{
	CLASSDECLARATION(CMeshCollider)

public:
	void Init() override;
	void Update() override;

	void SetProperty(const String &key, const String &value) override;
};