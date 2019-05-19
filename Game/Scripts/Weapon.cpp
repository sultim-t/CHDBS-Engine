#include "Weapon.h"
#include <Engine/Input/Input.h>
#include <Engine/SceneManager/SceneManager.h>
#include <Engine/Systems/PhysicsSystem.h>
#include <Engine/Physics/RaycastInfo.h>
#include <Engine/Physics/CollisionInfo.h>
#include <Engine/Math/Ray.h>
#include <Engine/Components/CParticleSystem.h>
#include <Engine/Components/CSphereCollider.h>
#include <Engine/Components/CVertexAnimated.h>
#include <Engine/Systems/UISystem.h>

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

	animatedModel = owner->GetComponent<CVertexAnimated>();

	weaponTimer = reloadingTime;
	shotAmmoCount = 2;
	ammoCount = 120;

	InitWeaponUI();
}

void CWeapon::InitWeaponUI()
{
	uiText = UISystem::Instance().AddText("Weapon");

	uiText->GetTransform().SetPosition(Vector2(1100, 25));
	uiText->BindFont("Fonts\\Roboto-Regular.ttf");

	char s[16];
	snprintf(s, 16, "%d", ammoCount);
	uiText->Text = s;
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
		if (animatedModel != nullptr)
		{
			animatedModel->PlayAnimation(0, 0.0f);
		}

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

		ammoCount -= shotAmmoCount;

		char s[16];
		snprintf(s, 16, "%d", ammoCount);
		uiText->Text = s;
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

		if (info.HittedRigidbody != nullptr)
		{
			info.HittedRigidbody->AddImpulse(dir.GetNormalized() * 5000.0f);
			particles->Emit(15, info.Normal, Color4(255, 0, 0, 255));
		}
		else
		{
			particles->Emit(15, info.Normal);
		}
	}
}

void CWeapon::ShootShotgun()
{
	const Transform &transform = GetOwner().GetTransform();

	for (int i = 0; i < 7; i++)
	{
		Vector3 localDir = Vector3((i - 3) / 3.0f * 0.2f, (i % 2 - 0.5f) * 0.05f, 1);
		Vector3 dir = transform.DirectionFromLocal(localDir);
		RaycastInfo info;

		if (PhysicsSystem::Instance().Raycast(transform.GetPosition(), dir, info))
		{
			particles->GetOwner().GetTransform().SetPosition(info.Point);

			if (info.HittedRigidbody != nullptr)
			{
				info.HittedRigidbody->AddImpulse(dir.GetNormalized() * 5000.0f);
				particles->Emit(15, info.Normal, Color4(255, 0, 0, 255));
			}
			else
			{
				particles->Emit(15, info.Normal);
			}
		}
	}
}

void CWeapon::OnCollision(const CollisionInfo *info)
{
	if (particles == nullptr)
	{
		return;
	}

	for (int i = 0; i < info->ContactsCount; i++)
	{
		particles->GetOwner().GetTransform().SetPosition(info->Contacts[i].Point);
		particles->Emit(1, info->Contacts[i].Normal);
	}
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
