#include "CCamera.h"

#include <Engine/Math/Transform.h>
#include <Engine/Math/Projection.h>
#include <Engine/Entities/Entity.h>
#include <Engine/Systems/RenderingSystem.h>
#include <Engine/Rendering/DebugDrawer.h>

#define PROPERTY_KEY_PROJ	"Projection"
#define PROPERTY_KEY_ZNEAR	"ZNear"
#define PROPERTY_KEY_ZFAR	"ZFar"
#define PROPERTY_KEY_FOV	"FOV"
#define PROPERTY_KEY_ORTHL	"OrthoL"
#define PROPERTY_KEY_ORTHR	"OrthoR"
#define PROPERTY_KEY_ORTHU	"OrthoU"
#define PROPERTY_KEY_ORTHB	"OrthoB"

#define PROPERTY_VAL_PROJORTHO	"Ortho"
#define PROPERTY_VAL_PROJPERSP	"Perspective"

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
	//RenderingSystem::Instance().Register(this);

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
	if (aspect <= 0)
	{
		aspect = 1.0f;
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

const Transform & CCamera::GetTransform() const
{
	return owner->GetTransform();
}

Matrix4 CCamera::GetViewMatrix() const
{
	const Transform &t = owner->GetTransform();
	const Vector3 &position = t.GetPosition();

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

CameraProjection CCamera::GetProjection() const
{
	return projection;
}

float CCamera::GetFOV() const
{
	return fov;
}

float CCamera::GetNearClipDist() const
{
	return zNear;
}

float CCamera::GetFarClipDist() const
{
	return zFar;
}

float CCamera::GetAspect() const
{
	return aspect;
}

Frustum CCamera::GetFrustum() const
{
	return GetFrustum(zNear, zFar);
}

Frustum CCamera::GetFrustum(float newNear, float newFar) const
{
	Frustum frustum;

	if (projection == CameraProjection::Perspective)
	{
		// init frustum's vertices
		frustum.Init(fov, aspect, newNear, newFar, GetOwner().GetTransform().GetTransformMatrix());
	}
	else
	{
		// todo
		Logger::Print("Ortho camera frustum not implemented");
	}

	return frustum;
}

void CCamera::SetProjection(CameraProjection p)
{
	this->projection = p;
}

void CCamera::SetFOV(float fov)
{
	ASSERT(fov > 0);
	ASSERT(projection == CameraProjection::Perspective);

	this->fov = fov;
}

void CCamera::SetNearClipDist(float zNear)
{
	ASSERT(zNear > 0);
	this->zNear = zNear;
}

void CCamera::SetFarClipDist(float zFar)
{
	ASSERT(zFar > 0);
	this->zFar = zFar;
}

void CCamera::SetAspect(float width, float height)
{
	aspect = width / height;
}