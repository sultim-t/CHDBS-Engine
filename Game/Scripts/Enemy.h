//#pragma once
//#include "IDamageable.h"
//#include <Engine/Components/IComponent.h>
//#include <Engine/Rendering/IAnimatable.h>
//
//enum class EnemyAttackType
//{
//	Melee,
//	Gun
//};
//
//class CEnemy : public IComponent, public IDamageable
//{
//	CLASSDECLARATION(CEnemy)
//
//private:
//	float			health;
//
//	float			speed;
//
//	float			attackDamage;
//	EnemyAttackType attackType;
//
//	Rigidbody		*rigidbody;
//	IAnimatable		*animatable;
//	CParticleSystem *damageParticles;
//
//	Entity			*target;
//
//public:
//	void OnSceneLoading(int sceneId) override;
//
//	void FixedUpdate();
//
//	void Attack();
//
//	float GetHealth() const;
//	void ReceiveDamage(const Damage &damage) override;
//
//	void Init() override;
//	void Update() override;
//	void SetProperty(const String &key, const String &value) override;
//};