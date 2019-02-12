#include "CCamera.h"

#include <Engine/Math/Transform.h>
#include <Engine/Math/Projection.h>
#include <Engine/Entities/Entity.h>
#include <Engine/Base/String.h>

#define PROPERTY_KEY_PROJ	"proj"
#define PROPERTY_KEY_ZNEAR	"znear"
#define PROPERTY_KEY_ZFAR	"zfar"
#define PROPERTY_KEY_FOV	"fov"
#define PROPERTY_KEY_ORTHL	"orthl"
#define PROPERTY_KEY_ORTHR	"orthr"
#define PROPERTY_KEY_ORTHU	"orthu"
#define PROPERTY_KEY_ORTHB	"orthb"

#define PROPERTY_VAL_PROJORTHO	"ortho"
#define PROPERTY_VAL_PROJPERSP	"persp"

#define DEFAULT_PROJ	CAMERA_PROJ_PERSPECTIVE;
#define DEFAULT_FOV		80.0f;
#define DEFAULT_ZNEAR	0.1f;
#define DEFAULT_ZFAR	100.0f;
#define DEFAULT_ORTHO_L -10.0f
#define DEFAULT_ORTHO_R 10.0f
#define DEFAULT_ORTHO_U 10.0f
#define DEFAULT_ORTHO_B -10.0f

CLASSDEFINITION(IComponent, CCamera)

void CCamera::Init()
{ }

void CCamera::SetProperty(const String & key, const String & value)
{
	if (key == PROPERTY_KEY_PROJ)
	{
		if (value == PROPERTY_VAL_PROJORTHO)
		{
			projection = CAMERA_PROJ_ORTHOGRAPHIC;
		}
		else if (value == PROPERTY_VAL_PROJPERSP)
		{
			projection = CAMERA_PROJ_PERSPECTIVE;
		}
		else
		{
			Logger::Print("Wrong camera projection");
			projection = DEFAULT_PROJ;
		}
	}
	else if (key == PROPERTY_KEY_ZNEAR)
	{
		zNear = value.ToFloat();
	}
	else if (key == PROPERTY_KEY_ZFAR)
	{
		zFar = value.ToFloat();
	}
	else if (key == PROPERTY_KEY_FOV)
	{
		fov = value.ToFloat();
	}
	else if (key == PROPERTY_KEY_ORTHL)
	{
		orthoLeft = value.ToFloat();
	}
	else if (key == PROPERTY_KEY_ORTHR)
	{
		orthoRight = value.ToFloat();
	}
	else if (key == PROPERTY_KEY_ORTHU)
	{
		orthoUp = value.ToFloat();
	}
	else if (key == PROPERTY_KEY_ORTHB)
	{
		orthoBottom = value.ToFloat();
	}
	else
	{
		Logger::Print("Wrong camera property");
	}
}

Matrix4 CCamera::GetViewMatrix() const
{
	Transform t = owner->GetTransform();
	Vector3 position = t.GetPosition();

	Vector3 f = t.GetForward();
	Vector3 r = t.GetRight();
	Vector3 u = t.GetUp();
	
	return Transform::LookAt(position, position + f, u);
}

Matrix4 CCamera::GetProjectionMatrix(float width, float height)
{
	if (projection == CAMERA_PROJ_PERSPECTIVE)
	{
		return Projection::Perspective(fov, width / height, zNear, zFar);
	}
	else
	{
		return Projection::Ortho(orthoLeft, orthoRight, orthoBottom, orthoUp, zNear, zFar);
	}
}