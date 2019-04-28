#include "Weapon.h"
#include <Engine/Input/Input.h>
#include <Engine/Components/CParticleSystem.h>
#include <Engine/SceneManager/SceneManager.h>
#include <Engine/Systems/PhysicsSystem.h>
#include <Engine/Physics/RaycastInfo.h>
#include <Engine/Math/Ray.h>
#include <Engine/Physics/CollisionInfo.h>
#include <Engine/Components/CSphereCollider.h>

CLASSDEFINITION(IComponent, CWeapon)

void CWeapon::Init()
{
	if (reloadingTime <= 0.0f)
	{
		reloadingTime = 1.0f;
	}

	particles = nullptr;

	thisCollider = owner->GetComponent<CSphereCollider>();

	if (thisCollider != nullptr)
	{
		thisCollider->OnCollisionEnter().Subscribe<CWeapon>(this, &CWeapon::OnCollision);
	}
}

void CWeapon::Update()
{
	// update timer
	weaponTimer += Time::GetDeltaTime();

	// if weapon is reloading
	if (weaponTimer < reloadingTime)
	{
		return;
	}

	if (particles == nullptr)
	{
		Entity *particleEntity = SceneManager::Instance().GetCurrentScene().FindEntity("Particles");
		particles = particleEntity->GetComponent<CParticleSystem>();

		// can't find
		if (particles == nullptr)
		{
			return;
		}
	}

	// if key is pressed
	if (Input::IsPressedMouse(Keycode::MouseBUTTON1))
	{
		if (weaponType == WeaponType::Shotgun)
		{
			ShootShotgun();
		}
		else
		{
			Shoot();
		}

		// reset timer
		weaponTimer = 0;
	}
}
void CWeapon::Shoot()
{
	const Transform &transform = GetOwner().GetTransform();

	Vector3 localDir = Vector3(0, 0, 1);
	Vector3 dir = transform.DirectionFromLocal(localDir);
	RaycastInfo info;

	if (PhysicsSystem::Instance().Raycast(transform.GetPosition(), dir, info))
	{
		particles->GetOwner().GetTransform().SetPosition(info.Point);
		particles->Emit(15, info.Normal);

		if (info.HittedRigidbody != nullptr)
		{
			// info.HittedRigidbody->AddImpulse(dir.GetNormalized() * 1000.0f);
		}
	}
}

void CWeapon::ShootShotgun()
{
	const Transform &transform = GetOwner().GetTransform();

	for (int i = 0; i < 7; i++)
	{
		Vector3 localDir = Vector3((i - 3) / 3.0f * 0.2f, (i % 2 - 0.5f) * 0.1f, 1);
		Vector3 dir = transform.DirectionFromLocal(localDir);
		RaycastInfo info;

		if (PhysicsSystem::Instance().Raycast(transform.GetPosition(), dir, info))
		{
			particles->GetOwner().GetTransform().SetPosition(info.Point);
			particles->Emit(15, info.Normal);

			if (info.HittedRigidbody != nullptr)
			{
				// info.HittedRigidbody->AddImpulse(dir.GetNormalized() * 1000.0f);
			}
		}
	}
}

void CWeapon::OnCollision(const CollisionInfo * info)
{
	if (particles == nullptr)
	{
		return;
	}

	particles->GetOwner().GetTransform().SetPosition(info->Contact.Point);
	particles->Emit(15, info->Contact.Normal);
}

#define PROPERTY_KEY_RELOADTIME			"ReloadingTime"	
#define PROPERTY_KEY_WEAPONTYPE			"WeaponType"	

#define PROPERTY_VALUE_WEAPONGUN		"Gun"
#define PROPERTY_VALUE_WEAPONSHOTGUN	"Shotgun"	

void CWeapon::SetProperty(const String & key, const String & value)
{
	if (key == PROPERTY_KEY_RELOADTIME)
	{
		reloadingTime = value.ToFloat();
	}
	else if (key == PROPERTY_KEY_WEAPONTYPE)
	{
		if (value == PROPERTY_VALUE_WEAPONGUN)
		{
			weaponType = WeaponType::Gun;
		}
		else if (value == PROPERTY_VALUE_WEAPONSHOTGUN)
		{
			weaponType = WeaponType::Shotgun;
		}
	}
}
