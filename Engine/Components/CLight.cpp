#include "CLight.h"
#include <Engine/Math/Projection.h>
#include <Engine/Entities/Entity.h>
#include <Engine/Systems/RenderingSystem.h>

#define PROPERTY_KEY_TYPE		"type"
#define PROPERTY_KEY_COLORR		"colorr"
#define PROPERTY_KEY_COLORG		"colorg"
#define PROPERTY_KEY_COLORB		"colorb"
#define PROPERTY_KEY_BIAS		"bias"
#define PROPERTY_KEY_STATIC		"static"
#define PROPERTY_KEY_SHADOWS	"shadows"
#define PROPERTY_KEY_CONEANGLE	"angle"
#define PROPERTY_KEY_RANGE		"range"

#define PROPERTY_VAL_TYPEDIR	"dir"
#define PROPERTY_VAL_TYPEPOINT	"point"
#define PROPERTY_VAL_TYPESPOT	"spot"
#define PROPERTY_VAL_TYPEAMBIENT "ambient"

CLASSDEFINITION(IComponent, CLight)

void CLight::Recalculate()
{
	lightSpace = GetLightView() * GetProjection();
}

Matrix4 CLight::GetLightView() const
{
	return GetLightView(owner->GetTransform().GetPosition());
}

Matrix4 CLight::GetLightView(const Vector3 &pos) const
{
	Vector3 dir = owner->GetTransform().GetForward();
	return Transform::LookAt(pos, pos + dir, Vector3(0.0f, 1.0f, 0.0f));
}

Matrix4 CLight::GetProjection() const
{
	if (ltype == LightType::Directional)
	{
		return Projection::Ortho(-0, 30, -30, 30, 0.1f, 200);
	}
	else if (ltype == LightType::Spot)
	{
		return Projection::Perspective(coneAngle, 1, 0.1f, range);
	}
	else
	{
		ASSERT(0);
		return Matrix4();
	}
}

LightType CLight::GetLightType() const
{
	return ltype;
}

const Matrix4 &CLight::GetLightSpace() const
{
	return lightSpace;
}

Matrix4 CLight::GetLightSpace(const Vector3 &pos) const
{
	return GetLightView() * GetProjection();
}

bool CLight::IsStatic() const
{
	return isStatic;
}

const Vector3 &CLight::GetColor() const
{
	return color;
}

float CLight::GetBias() const
{
	return bias;
}

float CLight::GetRange() const
{
	return range;
}

float CLight::GetConeAngle() const
{
	return coneAngle;
}

bool CLight::IsCastingShadows() const
{
	return castShadows;
}

Vector3 CLight::GetPosition() const
{
	return owner->GetTransform().GetPosition();
}

void CLight::SetLightType(LightType type)
{
	this->ltype = type;
}

void CLight::SetStatic(bool isStatic)
{
	this->isStatic = isStatic;
}

void CLight::SetColor(const Color &color)
{
	this->color = color;
}

void CLight::SetBias(float bias)
{
	this->bias = bias;
}

void CLight::SetRange(float range)
{
	this->range = range;
}

void CLight::SetConeAngle(float coneAngle)
{
	this->coneAngle = coneAngle > 0 ? coneAngle : 0;
}

void CLight::SetCastingShadows(bool cast)
{
	this->castShadows = cast;
}

void CLight::Init()
{
	RenderingSystem::Instance().Register(this);

	// if light is static just calculate matrix one time
	if (isStatic)
	{
		Recalculate();
	}
}

void CLight::Update()
{
	// don't update static lights
	if (isStatic)
	{
		return;
	}

	Recalculate();
}

void CLight::SetProperty(const String &key, const String &value)
{
	if (key == PROPERTY_KEY_TYPE)
	{
		if (value == PROPERTY_VAL_TYPEDIR)
		{
			ltype = LightType::Directional;
		}
		else if (value == PROPERTY_VAL_TYPEPOINT)
		{
			ltype = LightType::Point;
		}
		else if (value == PROPERTY_VAL_TYPESPOT)
		{
			ltype = LightType::Spot;
		}
		else if (value == PROPERTY_VAL_TYPEAMBIENT)
		{
			ltype = LightType::Ambient;
		}
		else
		{
			Logger::Print("Wrong light type");
		}
	}
	else if (key == PROPERTY_KEY_COLORR)
	{
		color[0] = value.ToFloat();
	}
	else if (key == PROPERTY_KEY_COLORG)
	{
		color[1] = value.ToFloat();
	}
	else if (key == PROPERTY_KEY_COLORB)
	{
		color[2] = value.ToFloat();
	}
	else if (key == PROPERTY_KEY_BIAS)
	{
		bias = value.ToFloat();
	}
	else if (key == PROPERTY_KEY_STATIC)
	{
		isStatic = value.ToBool();
	}
	else if (key == PROPERTY_KEY_SHADOWS)
	{
		castShadows = value.ToBool();
	}
	else if (key == PROPERTY_KEY_CONEANGLE)
	{
		coneAngle = value.ToFloat();
	}
	else if (key == PROPERTY_KEY_RANGE)
	{
		range = value.ToFloat();
	}
	else
	{
		Logger::Print("Wrong camera property");
	}
}
