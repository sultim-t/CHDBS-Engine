#pragma once
#include <Engine/Components/IComponent.h>
#include <Engine/Components/CParticleSystem.h>
#include <Engine/Base/Event.h>
#include <Engine/Rendering/GUI/UIText.h>

enum class WeaponType
{
	Gun,
	Shotgun
};

class CWeapon : public IComponent
{
	CLASSDECLARATION(CWeapon)

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

	int shotAmmoCount;
	int ammoCount;

	// UI text
	UIText *uiText;

private:
	void Shoot();
	void ShootShotgun();
	void CheckRay(const Vector3 &pos, const Vector3 &dir);

	void OnCollision(const CollisionInfo *info);

	void InitWeaponUI();

public:
	void Init() override;
	void Update() override;

	void SetProperty(const String &key, const String &value) override;
};