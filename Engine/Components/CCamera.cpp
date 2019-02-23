#include "CCamera.h"

#include <Engine/Math/Transform.h>
#include <Engine/Math/Projection.h>
#include <Engine/Entities/Entity.h>
#include <Engine/Systems/RenderingSystem.h>

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

#define DEFAULT_FOV		80.0f;
#define DEFAULT_ZNEAR	0.1f;
#define DEFAULT_ZFAR	100.0f;
#define DEFAULT_ORTHO_L -10.0f
#define DEFAULT_ORTHO_R 10.0f
#define DEFAULT_ORTHO_U 10.0f
#define DEFAULT_ORTHO_B -10.0f

CLASSDEFINITION(IComponent, CCamera)

void CCamera::Init()
{ 
	RenderingSystem::Instance().Register(this);

	if (zNear <= 0.0f)
	{
		zNear = DEFAULT_ZNEAR;
	}
	if (zFar <= 0.0f)
	{
		zFar = DEFAULT_ZFAR;
	}
	if (fov <= 0)
	{
		fov = DEFAULT_FOV;
	}
}

void CCamera::SetProperty(const String &key, const String &value)
{
	if (key == PROPERTY_KEY_PROJ)
	{
		if (value == PROPERTY_VAL_PROJORTHO)
		{
			projection = CameraProjection::Orthographic;
		}
		else if (value == PROPERTY_VAL_PROJPERSP)
		{
			projection = CameraProjection::Perspective;
		}
		else
		{
			Logger::Print("Wrong camera projection");
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

	// opengl uses inverse z axis
	Vector3 f = -t.GetForward();
	Vector3 u = t.GetUp();
	
	return Transform::LookAt(position, position + f, u);
}

Matrix4 CCamera::GetProjectionMatrix() const
{
	ASSERT(aspect != 0.0f);

	if (projection == CameraProjection::Perspective)
	{
		return Projection::Perspective(fov, aspect, zNear, zFar);
	}
	else
	{
		return Projection::Ortho(orthoLeft, orthoRight, orthoBottom, orthoUp, zNear, zFar);
	}
}