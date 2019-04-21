#pragma once
#include <Engine/Components/IComponent.h>
#include <Engine/Math/Quaternion.h>
#include <Engine/Math/Matrix.h>
#include <Engine/Rendering/ILight.h>

enum class LightType
{
	Directional,
	Point,
	Spot
};

class CLight : public IComponent, public ILight
{
	CLASSDECLARATION(CLight)

private:
	LightType ltype;
		
	Color3F color;
	float bias;

	bool castShadows;

	// for spotlights
	float coneAngle;

	// for spotlights and point lights
	float attenuation;

private:
	Matrix4 GetLightView() const;
	Matrix4 GetLightView(const Vector3 &pos) const;
	Matrix4 GetProjection(const AABB &bounding) const;

	// Get bounding box of frustum in light space
	AABB GetFrustumBounding(const Frustum &frustum) const;

public:
	// Returns light space matrix
	// View * Projection, used for shadow mapping
	// Used for rendering from light's point of view
	// If light is directional, it will fit the frustum
	const Matrix4 GetLightSpace(const Frustum &frustum) const;

	LightType GetLightType() const;
	void SetLightType(LightType type);
		
	// Returns light bias
	float GetBias() const;
	void SetBias(float bias);

	void SetCastingShadows(bool cast);
	void SetColor(const Color3F &color);
	void SetAttenuation(float a);
	void SetConeAngle(float a);

public:
	Vector4 GetPosition() const override;
	Color3F GetColor() const override;
	float GetAttenuation() const override;
	float GetConeAngle() const override;
	Vector3 GetConeDirection() const override;
	bool IsCastingShadows() const override;

public:
	void Init() override;
	void Update() override;

	void SetProperty(const String &key, const String &value) override;
};


//Color3F GetColor() const;
//// Returns range of light
//float GetRange() const;
//// Returns cone angle of spotlight in degrees
//float GetConeAngle() const;
//// Returns postion
//Vector3 GetPosition() const;
//
//void SetColor(const Color3F &color);
//// Set range of light (only point and spot)
//void SetRange(float range);
//// Set cone angle of light (only spot)
//void SetConeAngle(float coneAngle);
