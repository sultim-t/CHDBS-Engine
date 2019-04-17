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
#define PROPERTY_KEY_STATIC		"IsStatic"
#define PROPERTY_KEY_SHADOWS	"Shadows"
#define PROPERTY_KEY_CONEANGLE	"Angle"
#define PROPERTY_KEY_RANGE		"Range"

#define PROPERTY_VAL_TYPEDIR	"Directional"
#define PROPERTY_VAL_TYPEPOINT	"Point"
#define PROPERTY_VAL_TYPESPOT	"Spot"

CLASSDEFINITION(IComponent, CLight)

Matrix4 CLight::GetLightView() const
{
	return GetLightView(owner->GetTransform().GetPosition());
}

Matrix4 CLight::GetLightView(const Vector3 &pos) const
{
	Vector3 dir = owner->GetTransform().GetForward();
	return Transform::LookAt(pos, pos + dir, Vector3(0.0f, 1.0f, 0.0f));
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
		return Projection::Perspective(coneAngle, 1, 0.1f, range);
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
	if (ltype == LightType::Directional)
	{
		// bounding box for frustum in light space
		AABB frustumBounding = GetFrustumBounding(frustum);

		// get center of box in global space
		Vector3 pos = owner->GetTransform().PointFromLocal(frustumBounding.GetCenter());

		DebugDrawer::Instance().Draw(frustumBounding, owner->GetTransform().GetTransformMatrix(), Color4(255,0,0));

		AABB a;
		a.SetMin(frustumBounding.GetMin() * 0.3f);
		a.SetMax(frustumBounding.GetMax() * 0.3f);

		DebugDrawer::Instance().Draw(a, owner->GetTransform().GetTransformMatrix(), Color4(0, 0, 255));

		return GetLightView(pos) * GetProjection(frustumBounding);
	}
	else
	{
		Vector3 pos = owner->GetTransform().GetPosition();

		ASSERT(0);
		return 0;
	}
}

bool CLight::IsStatic() const
{
	return isStatic;
}

const Color3F &CLight::GetColor() const
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

void CLight::SetColor(const Color3F &color)
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
