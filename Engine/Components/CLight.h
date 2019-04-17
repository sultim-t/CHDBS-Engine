#pragma once
#include <Engine/Components/IComponent.h>
#include <Engine/Math/Quaternion.h>
#include <Engine/Math/Matrix.h>

enum class LightType
{
	Directional,
	Point,
	Spot,
	Ambient
};

class CLight : public IComponent
{
	CLASSDECLARATION(CLight)

private:
	LightType ltype;
		
	Color3F color;
	float bias;

	bool isStatic;
	bool castShadows;

	// for spotlights
	float coneAngle;

	// for spotlights and point lights
	float range;

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
	// If static light doesn't calculate matrix each frame
	bool IsStatic() const;
	const Color3F &GetColor() const;
	// Returns light bias
	float GetBias() const;
	// Returns range of light
	float GetRange() const;
	// Returns cone angle of spotlight in degrees
	float GetConeAngle() const;
	bool IsCastingShadows() const;
	// Returns postion
	Vector3 GetPosition() const;

	void SetLightType(LightType type);
	// If static light doesn't calculate matrix each frame
	void SetStatic(bool isStatic);
	void SetColor(const Color3F &color);
	void SetBias(float bias);
	// Set range of light (only point and spot)
	void SetRange(float range);
	// Set cone angle of light (only spot)
	void SetConeAngle(float coneAngle);
	void SetCastingShadows(bool cast);

public:
	void Init() override;
	void Update() override;

	void SetProperty(const String &key, const String &value) override;
};