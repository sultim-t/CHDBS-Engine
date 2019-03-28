#pragma once

enum class PhysicMaterialCombine
{
	Average,
	Minimum,
	Maximum,
	Multiply
};

class PhysicMaterial
{
private:
	// Restitution of the material
	float restitution;
	// Friction of the material
	float friction;

	// How friction of 2 materials is combined
	PhysicMaterialCombine frictionCombine;
	// How friction of 2 materials is combined
	PhysicMaterialCombine restitutionCombine;

public:
	// Default constructor
	inline PhysicMaterial();
	// Consructor with default combine options
	inline PhysicMaterial(float friction, float restitution);
	// Consructor with specified options
	inline PhysicMaterial(float friction, float restitution, PhysicMaterialCombine frictionCombine, PhysicMaterialCombine restitutionCombine);

	inline float GetFriction() const;
	inline float GetRestitution() const;
	inline PhysicMaterialCombine GetFrictionCombine() const;
	inline PhysicMaterialCombine GetRestitutionCombine() const;

	inline void SetFriction(float friction);
	inline void SetRestitution(float restitution);
	inline void SetFrictionCombine(PhysicMaterialCombine frictionCombine);
	inline void SetRestitutionCombine(PhysicMaterialCombine restitutionCombine);
};

inline PhysicMaterial::PhysicMaterial() :
	friction(0.5f),
	restitution(0.5f),
	frictionCombine(PhysicMaterialCombine::Average),
	restitutionCombine(PhysicMaterialCombine::Average) { }

inline PhysicMaterial::PhysicMaterial(float friction, float restitution) :
	friction(friction),
	restitution(restitution),
	frictionCombine(PhysicMaterialCombine::Average),
	restitutionCombine(PhysicMaterialCombine::Average) { }

inline PhysicMaterial::PhysicMaterial(float friction, float restitution, PhysicMaterialCombine frictionCombine, PhysicMaterialCombine restitutionCombine) :
	friction(friction),
	restitution(restitution),
	frictionCombine(frictionCombine),
	restitutionCombine(restitutionCombine) { }

inline float PhysicMaterial::GetFriction() const
{
	return friction;
}

inline float PhysicMaterial::GetRestitution() const
{
	return restitution;
}

inline PhysicMaterialCombine PhysicMaterial::GetFrictionCombine() const
{
	return frictionCombine;
}

inline PhysicMaterialCombine PhysicMaterial::GetRestitutionCombine() const
{
	return restitutionCombine;
}

inline void PhysicMaterial::SetFriction(float friction)
{
	ASSERT(friction >= 0.0f && friction <= 1.0f);
	this->friction = friction;
}

inline void PhysicMaterial::SetRestitution(float restitution)
{
	ASSERT(restitution >= 0.0f && restitution <= 1.0f);
	this->restitution = restitution;
}

inline void PhysicMaterial::SetFrictionCombine(PhysicMaterialCombine frictionCombine)
{
	this->frictionCombine = frictionCombine;
}

inline void PhysicMaterial::SetRestitutionCombine(PhysicMaterialCombine restitutionCombine)
{
	this->restitutionCombine = restitutionCombine;
}