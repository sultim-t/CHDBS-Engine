//#include "Enemy.h"
//#include <Engine/Components/CParticleSystem.h>
//#include <Engine/Components/CVertexAnimated.h>
//#include <Engine/Physics/Rigidbody.h>
//#include <Engine/SceneManager/SceneManager.h>
//
//#define ENEMY_ANIM_IDLE		0
//#define ENEMY_ANIM_WALK		1
//#define ENEMY_ANIM_ATTACK	2
//#define ENEMY_ANIM_DAMAGE	3
//#define ENEMY_ANIM_DEATH	4
//
//void CEnemy::Init()
//{ }
//
//void CEnemy::OnSceneLoading(int sceneId)
//{
//	rigidbody = GetOwner().GetComponent<Rigidbody>();
//	animatable = GetOwner().GetComponent<CVertexAnimated>();
//	damageParticles = GetOwner().GetComponent<CParticleSystem>();
//	target = SceneManager::Instance().GetScene(sceneId).FindEntity("Player");
//
//	SceneManager::Instance().GetScene(sceneId).GetFixedUpdateEvent().Subscribe(this, CEnemy::FixedUpdate);
//}
//
//void CEnemy::Update()
//{
//	int currentAnim = animatable->GetCurrentAnimation();
//	
//	// if damaging, dying or attacking
//	if (currentAnim == ENEMY_ANIM_DAMAGE || currentAnim == ENEMY_ANIM_DEATH || currentAnim == ENEMY_ANIM_ATTACK)
//	{
//		// wait
//		return;
//	}
//
//	// if target exist
//	if (target != nullptr)
//	{ 
//		if (currentAnim != ENEMY_ANIM_WALK)
//		{
//			animatable->PlayAnimation(ENEMY_ANIM_WALK);
//		}
//
//		return;
//	}
//	
//	// else idle
//	if (currentAnim != ENEMY_ANIM_IDLE)
//	{
//		animatable->PlayAnimation(ENEMY_ANIM_IDLE);
//	}
//}
//
//void CEnemy::FixedUpdate()
//{
//	int currentAnim = animatable->GetCurrentAnimation();
//
//	if (currentAnim == ENEMY_ANIM_WALK)
//	{
//		Vector3 dir = target->GetTransform().GetPosition() - GetOwner().GetTransform().GetPosition();
//		rigidbody->GetVelocity() = dir * speed;
//	}
//}
//
//void CEnemy::Attack()
//{
//	rigidbody->GetVelocity() = 0.0f;
//
//	if (attackType == EnemyAttackType::Melee)
//	{
//
//	}
//	else if (attackType == EnemyAttackType::Gun)
//	{
//
//	}
//}
//
//float CEnemy::GetHealth() const
//{
//	return health;
//}
//
//void CEnemy::ReceiveDamage(const Damage &damage)
//{
//	health -= damage.Value;
//
//	damageParticles->GetOwner().GetTransform().SetPosition(damage.Position);
//	damageParticles->Emit(15, -damage.Direction, Color4(255, 0, 0, 255));
//
//	if (health > 0.0f)
//	{
//		rigidbody->GetVelocity() = 0.0f;
//		animatable->PlayAnimation(ENEMY_ANIM_DAMAGE);
//		rigidbody->AddImpulse(damage.Direction * damage.Value);
//	}
//	else
//	{
//		rigidbody->GetVelocity() = 0.0f;
//		health = 0.0f;
//		animatable->PlayAnimation(ENEMY_ANIM_DEATH);
//		rigidbody->AddImpulse(damage.Direction * damage.Value * 5);
//	}
//}
//
//#define PROPERTY_KEY_HEALTH			"Health"	
//#define PROPERTY_KEY_SPEED			"Speed"	
//#define PROPERTY_KEY_ATTACK_VALUE	"AttackDamage"	
//#define PROPERTY_KEY_ATTACK_TYPE	"AttackType"	
//
//#define PROPERTY_VALUE_ATCK_MELEE	"Melee"
//#define PROPERTY_VALUE_ATCK_GUN		"Gun"	
//
//void CEnemy::SetProperty(const String &key, const String &value)
//{
//	if (key == PROPERTY_KEY_HEALTH)
//	{
//		health = value.ToFloat();
//	}
//	else if (key == PROPERTY_KEY_ATTACK_VALUE)
//	{
//		attackDamage = value.ToFloat();
//	}
//	else if (key == PROPERTY_KEY_SPEED)
//	{
//		speed = value.ToFloat();
//	}
//	else if (key == PROPERTY_KEY_ATTACK_TYPE)
//	{
//		if (value == PROPERTY_VALUE_ATCK_GUN)
//		{
//			attackType = EnemyAttackType::Gun;
//		}
//		else if (value == PROPERTY_VALUE_ATCK_MELEE)
//		{
//			attackType = EnemyAttackType::Melee;
//		}
//	}
//}
