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
	// Static friction of the material
	float staticFriction;
	// Dynamic friction of the material
	float dynamicFriction;

	// How friction of 2 materials is combined
	PhysicMaterialCombine frictionCombine;
	// How friction of 2 materials is combined
	PhysicMaterialCombine restitutionCombine;

public:
	// Default constructor
	inline PhysicMaterial();
	// Consructor with default combine options
	inline PhysicMaterial(float staticFriction, float dynamicFriction, float restitution);
	// Consructor with specified options
	inline PhysicMaterial(float friction, float restitution, float dynamicFriction, PhysicMaterialCombine frictionCombine, PhysicMaterialCombine restitutionCombine);

	inline float GetStaticFriction() const;
	inline float GetDynamicFriction() const;
	inline float GetRestitution() const;
	inline PhysicMaterialCombine GetFrictionCombine() const;
	inline PhysicMaterialCombine GetRestitutionCombine() const;

	inline void SetStaticFriction(float staticFriction);
	inline void SetDynamicFriction(float dynamicFriction);
	inline void SetRestitution(float restitution);
	inline void SetFrictionCombine(PhysicMaterialCombine frictionCombine);
	inline void SetRestitutionCombine(PhysicMaterialCombine restitutionCombine);
};

inline PhysicMaterial::PhysicMaterial() :
	staticFriction(0.5f),
	dynamicFriction(0.5f),
	restitution(0.5f),
	frictionCombine(PhysicMaterialCombine::Average),
	restitutionCombine(PhysicMaterialCombine::Average) { }

inline PhysicMaterial::PhysicMaterial(float staticFriction, float dynamicFriction, float restitution) :
	staticFriction(staticFriction),
	dynamicFriction(dynamicFriction),
	restitution(restitution),
	frictionCombine(PhysicMaterialCombine::Average),
	restitutionCombine(PhysicMaterialCombine::Average) { }

inline PhysicMaterial::PhysicMaterial(float staticFriction, float dynamicFriction, float restitution, PhysicMaterialCombine frictionCombine, PhysicMaterialCombine restitutionCombine) :
	staticFriction(staticFriction),
	dynamicFriction(dynamicFriction),
	restitution(restitution),
	frictionCombine(frictionCombine),
	restitutionCombine(restitutionCombine) { }

inline float PhysicMaterial::GetStaticFriction() const
{
	return staticFriction;
}

inline float PhysicMaterial::GetDynamicFriction() const
{
	return dynamicFriction;
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

inline void PhysicMaterial::SetStaticFriction(float staticFriction)
{
	ASSERT(staticFriction >= 0.0f && staticFriction <= 1.0f);
	this->staticFriction = staticFriction;
}

inline void PhysicMaterial::SetDynamicFriction(float dynamicFriction)
{
	ASSERT(dynamicFriction >= 0.0f && dynamicFriction <= 1.0f);
	this->dynamicFriction = dynamicFriction;
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