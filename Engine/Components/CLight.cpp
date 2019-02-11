#include "CLight.h"
#include <Engine/Math/Projection.h>
#include <Engine/Entities/Entity.h>
#include <Engine/Base/String.h>

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
	Matrix4 projection;

	if (ltype == LIGHT_DIRECTIONAL)
	{
		projection = Projection::Ortho(-10, 10, -10, 10, 0.1f, 30);
	}
	else if (ltype == LIGHT_SPOT)
	{
		projection = Projection::Perspective(coneAngle, 1, 0.1f, range);
	}
	else
	{ }

	Vector3 pos = owner->GetTransform().GetPosition();
	Vector3 at = pos + owner->GetTransform().GetForward();

	Matrix4 view = Transform::LookAt(pos, at, Vector3(0.0f, 1.0f, 0.0f));
	lightSpace = view * projection;
}

LightType CLight::GetLightType() const
{
	return ltype;
}

/*const Matrix4 & CLight::GetProjection() const
{
	return projection;
}

const Matrix4 & CLight::GetView() const
{
	return view;
}*/

const Matrix4 & CLight::GetLightSpace() const
{
	return lightSpace;
}

LightType & CLight::GetLightType()
{
	return ltype;
}

bool &CLight::IsStatic()
{
	return isStatic;
}

const bool CLight::IsStatic() const
{
	return isStatic;
}

const Vector3 & CLight::GetColor() const
{
	return color;
}

const float CLight::GetBias() const
{
	return bias;
}

const float CLight::GetRange() const
{
	return range;
}

const float CLight::GetConeAngle() const
{
	return coneAngle;
}

const bool CLight::IsCastingShadows() const
{
	return castShadows;
}

Vector3 CLight::GetPosition() const
{
	return owner->GetTransform().GetPosition();
}

Color & CLight::GetColor()
{
	return color;
}

float & CLight::GetBias()
{
	return bias;
}

const float CLight::GetRange()
{
	return range;
}

float & CLight::GetConeAngle()
{
	return coneAngle;
}

bool & CLight::IsCastingShadows()
{
	return castShadows;
}

void CLight::Init()
{
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

void CLight::SetProperty(const String & key, const String & value)
{
	if (key == PROPERTY_KEY_TYPE)
	{
		if (value == PROPERTY_VAL_TYPEDIR)
		{
			ltype = LIGHT_DIRECTIONAL;
		}
		else if (value == PROPERTY_VAL_TYPEPOINT)
		{
			ltype = LIGHT_POINT;
		}
		else if (value == PROPERTY_VAL_TYPESPOT)
		{
			ltype = LIGHT_SPOT;
		}
		else if (value == PROPERTY_VAL_TYPEAMBIENT)
		{
			ltype = LIGHT_AMBIENT;
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
		isStatic = value.ToInt() == 0 ? false : true;
	}
	else if (key == PROPERTY_KEY_SHADOWS)
	{
		castShadows = value.ToInt() == 0 ? false : true;
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
