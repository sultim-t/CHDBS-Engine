#include "CLight.h"
#include <Engine/Math/Projection.h>
#include <Engine/Math/Frustum.h>
#include <Engine/Entities/Entity.h>
#include <Engine/Systems/RenderingSystem.h>
#include <Engine/Math/AABB.h>
#include <Engine/Rendering/DebugDrawer.h>

#define PROPERTY_KEY_TYPE		"Type"
#define PROPERTY_KEY_COLOR		"Color"
#define PROPERTY_KEY_BIAS		"Bias"
#define PROPERTY_KEY_SHADOWS	"Shadows"
#define PROPERTY_KEY_CONEANGLE	"Angle"
#define PROPERTY_KEY_ATTEN		"Attenuation"

#define PROPERTY_VAL_TYPEDIR	"Directional"
#define PROPERTY_VAL_TYPEPOINT	"Point"
#define PROPERTY_VAL_TYPESPOT	"Spot"

CLASSDEFINITION(IComponent, CLight)

Matrix4 CLight::GetLightView() const
{
	return GetLightView(owner->GetTransform().GetPosition());
}

Matrix4 CLight::GetLightView(const Vector3 &position) const
{
	const Transform &t = owner->GetTransform();
	return Transform::LookAt(position, position + t.GetForward(), t.GetUp());
}

Matrix4 CLight::GetProjection(const AABB &bounding) const
{
	if (ltype == LightType::Directional)
	{
		// get bounding box
		const Vector3 &min = bounding.GetMin();
		const Vector3 &max = bounding.GetMax();

		// return projection matrix
		return Projection::Ortho(min[0], max[0], min[1], max[1], min[2], max[2]);
	}
	else if (ltype == LightType::Spot)
	{
		// return projection matrix
		return Projection::Perspective(coneAngle, 1, 0.1f, 20);
	}
	else
	{
		ASSERT(0);
		return Matrix4();
	}
}

AABB CLight::GetFrustumBounding(const Frustum &frustum) const
{
	// if light type is directional, calculate bounding box of frustum in light space

	const Vector3 *near = frustum.GetNearVerts();
	const Vector3 *far = frustum.GetFarVerts();

	// calculate positions in light space
	Vector3 verts[8];
	for (int i = 0; i < 4; i++)
	{
		verts[i] = owner->GetTransform().PointFromWorld(near[i]);
		verts[i + 4] = owner->GetTransform().PointFromWorld(far[i]);
	}

	// get bounding box
	Vector3 min(verts[0]);
	Vector3 max(verts[0]);
	for (int i = 1; i < 8; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			float c = verts[i][j];

			if (c < min[j])
			{
				min[j] = c;
			}
			else if (c > max[j])
			{
				max[j] = c;
			}
		}
	}

	// bounding box for frustum in light space
	return AABB(min, max);
}

LightType CLight::GetLightType() const
{
	return ltype;
}

const Matrix4 CLight::GetLightSpace(const Frustum &frustum) const
{
		// bounding box for frustum in light space
		AABB frustumBounding = GetFrustumBounding(frustum);

		// correct z min
		Vector3 min = frustumBounding.GetMin();
		min[2] -= 10.0f;

		// set new min
		frustumBounding.SetMin(min);

		// get center of box
		Vector3 pos = frustumBounding.GetCenter();
		// offset to min z
		pos[2] = min[2];
		// to global space
		pos = owner->GetTransform().PointFromLocal(pos);

		DebugDrawer::Instance().Draw(frustumBounding, owner->GetTransform().GetTransformMatrix(), Color4(255,0,0));
		DebugDrawer::Instance().Draw(Sphere(pos, 10), Color4(255, 0, 255));

		return GetLightView(pos) * GetProjection(frustumBounding);
}

float CLight::GetBias() const
{
	return bias;
}

float CLight::GetConeAngle() const
{
	// if a spot light
	if (ltype == LightType::Spot)
	{
		return coneAngle;

	}

	// return some value > 90
	return 180.0f;
}

Vector3 CLight::GetConeDirection() const
{
	return GetOwner().GetTransform().GetForward();
}

bool CLight::IsCastingShadows() const
{
	return castShadows;
}

void CLight::SetLightType(LightType type)
{
	this->ltype = type;
}

void CLight::SetColor(const Color3F &color)
{
	this->color = color;
}

void CLight::SetAttenuation(float a)
{
	this->attenuation = a;
}

void CLight::SetConeAngle(float a)
{
	this->coneAngle = a;
}

void CLight::SetBias(float bias)
{
	this->bias = bias;
}

void CLight::SetCastingShadows(bool cast)
{
	this->castShadows = cast;
}

void CLight::Init()
{
	//RenderingSystem::Instance().Register(this);
}

void CLight::Update()
{ }

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
		else
		{
			Logger::Print("Wrong light type");
		}
	}
	else if (key == PROPERTY_KEY_COLOR)
	{
		color = value.ToVector3();
	}
	else if (key == PROPERTY_KEY_BIAS)
	{
		bias = value.ToFloat();
	}
	else if (key == PROPERTY_KEY_SHADOWS)
	{
		castShadows = value.ToBool();
	}
	else if (key == PROPERTY_KEY_CONEANGLE)
	{
		coneAngle = value.ToFloat();
	}
	else if (key == PROPERTY_KEY_ATTEN)
	{
		attenuation = value.ToFloat();
	}
	else
	{
		String err = "Wrong light property: ";
		err += key + " -- " + value;
		Logger::Print(err);
	}
}

Vector4 CLight::GetPosition() const
{
	if (ltype == LightType::Directional)
	{
		// need to negate to convert to position
		return Vector4(-GetOwner().GetTransform().GetForward(), 0.0f);
	}
	else
	{
		return Vector4(GetOwner().GetTransform().GetPosition(), 1.0f);
	}
}

Color3F CLight::GetColor() const
{
	return color;
}

float CLight::GetAttenuation() const
{
	return attenuation;
}