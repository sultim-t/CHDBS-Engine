#pragma once
#include <Engine\Components\IComponent.h>
#include <Engine\Math\Quaternion.h>
#include <Engine\Math\Matrix.h>

enum LightType
{
	LIGHT_DIRECTIONAL,
	LIGHT_POINT,
	LIGHT_SPOT,
	LIGHT_AMBIENT
};

class CLight : public IComponent
{
	CLASSDECLARATION(CLight)

private:
	LightType ltype;
	
	// View * Projection, used for shadow mapping
	// Used for rendering from light's point of view
	Matrix4 lightSpace;
	
	Color color;
	float bias;

	bool isStatic;
	bool castShadows;

	// for spotlights
	float coneAngle;

	// for spotlights and point lights
	float range;

private:
	void Recalculate();
	
public:
	// Returns const reference to light space matrix
	const Matrix4 &GetLightSpace() const;

	// Returns light type
	LightType GetLightType() const;
	// Returns copy of bool
	const bool IsStatic() const;
	// Returns const reference of color
	const Color &GetColor() const;
	// Returns copy of bias
	const float GetBias() const;
	// Returns copy of range of light
	const float GetRange() const;
	// Returns copy of cone angle of spotlight in degrees
	const float GetConeAngle() const;
	const bool IsCastingShadows() const;
	// Returns postion
	Vector3 GetPosition() const;

	// Returns reference to light type
	LightType &GetLightType();
	// Returns reference to bool
	bool &IsStatic();
	// Returns reference to color
	Color &GetColor();
	// Returns reference to bias
	float &GetBias();
	// Returns reference to range of light
	const float GetRange();
	// Returns reference to cone angle of spotlight in degrees
	float &GetConeAngle();
	// Returns reference
	bool &IsCastingShadows();

public:
	void Init() override;
	void Update() override;

	void SetProperty(const String &key, const String &value) override;
};