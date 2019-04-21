#pragma once

class ILight
{
public:
	virtual Vector4	GetPosition() const = 0;
	virtual Color3F	GetColor() const = 0;
	virtual float	GetAttenuation() const = 0;
	virtual float	GetConeAngle() const = 0;
	virtual Vector3	GetConeDirection() const = 0;
	virtual bool	IsCastingShadows() const = 0;
};