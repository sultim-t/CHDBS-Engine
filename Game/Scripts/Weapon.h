#pragma once
#include <Engine/Components/IComponent.h>
#include <Engine/Components/CParticleSystem.h>
#include <Engine/Base/Event.h>

enum class WeaponType
{
	Gun,
	Shotgun
};

class CWeapon : public IComponent
{
	CLASSDECLARATION(CFreeMovement)

private:
	// Timer for reload check
	float weaponTimer;
	// Time to reload
	float reloadingTime;
	// Current weapon type
	WeaponType weaponType;

	// Particles to use
	CParticleSystem *particles;
	CVertexAnimated *animatedModel;
	CSphereCollider *thisCollider;

private:
	void Shoot();
	void ShootShotgun();

	void OnCollision(const CollisionInfo *info);

public:
	void Init() override;
	void Update() override;

	void SetProperty(const String &key, const String &value) override;
};