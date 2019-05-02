#pragma once
#include <Engine/Math/Vector.h>

enum class DamageType
{
	Default,
	Explosion
};

struct Damage
{
	Vector3		Position;
	Vector3		Direction;
	float		Value;
	DamageType	Type;
};

class IDamageable
{
public:
	virtual void ReceiveDamage(const Damage &damage) = 0;
};