#include "CFreeMovement.h"
#include <Engine/Input/Input.h>
#include <Engine/Rendering/ContextWindow.h>
#include <Engine/Entities/Entity.h>
#include <Engine/Components/CCamera.h>
#include <Engine/SceneManager/SceneManager.h>

#define MOVE_FORWARD	0
#define MOVE_BACK		1
#define MOVE_LEFT		2
#define MOVE_RIGHT		3

#define SCROLL_SPEED	20

#define PROPERTY_KEY_SPEED	"Speed"


CLASSDEFINITION(IComponent, CFreeMovement)

void CFreeMovement::Init()
{ }

void CFreeMovement::Update()
{
	if (Input::IsPressed(Keycode::KeyESCAPE))
	{
		ContextWindow::Instance().RequestClose();
	}

	float scroll = Input::IsPressed(Keycode::KeyKPSUBTRACT) ? -1.0f : 0.0f;
	scroll += Input::IsPressed(Keycode::KeyKPADD) ? 1.0f : 0.0f;

	ProcessMouseMovement(Input::MouseXOffset, Input::MouseYOffset);
	ProcessMouseScroll(scroll);
}

void CFreeMovement::OnSceneLoading(int sceneId)
{
	// subscribe fixed update function on this object
	SceneManager::Instance().GetScene(sceneId).GetFixedUpdateEvent().Subscribe(this, &CFreeMovement::FixedUpdate);
}

void CFreeMovement::SetProperty(const String &key, const String &value)
{
	if (key == PROPERTY_KEY_SPEED)
	{
		speed = value.ToFloat();
	}
	else
	{
		Logger::Print("Wrong CFreeMovement property");
	}
}

void CFreeMovement::ProcessKeyboard()
{
	Transform &t = owner->GetTransform();

	Vector3 offset = Vector3(0, 0, 0);

	Vector3 front = t.GetForward();
	Vector3 right = t.GetRight();
	Vector3 up = Vector3(0, 1, 0);

	float velocity = speed * Time::GetFixedDeltaTime();

	if (Input::IsPressed(Keycode::KeyW))
		offset += front * velocity;

	if (Input::IsPressed(Keycode::KeyS))
		offset -= front * velocity;

	if (Input::IsPressed(Keycode::KeyA))
		offset -= right * velocity;

	if (Input::IsPressed(Keycode::KeyD))
		offset += right * velocity;

	if (Input::IsPressed(Keycode::KeyQ))
		offset -= up * velocity;

	if (Input::IsPressed(Keycode::KeyE))
		offset += up * velocity;

	t.GetPosition() += offset;
}

void CFreeMovement::ProcessMouseMovement(float xoffset, float yoffset)
{
	using namespace Mathf;

	float ax = xoffset * Input::MouseSensitivity;
	float ay = yoffset * Input::MouseSensitivity;

	x = Lerp(x, x + ax, Time::GetDeltaTime() * 20);
	y = Lerp(y, y + ay, Time::GetDeltaTime() * 20);

	if (x > 180.0f)
	{
		x -= 360.0f;
	}

	if (x < -180.0f)
	{
		x += 360.0f;
	}

	if (y < -90.0f)
	{
		y = -90.0f;
	}
	else if (y > 90.0f)
	{
		y = 90.0f;
	}

	Euler e;
	e[PITCH] = x;
	e[YAW] = 0.0f;
	e[ROLL] = -y;

	owner->GetTransform().SetRotation(e);
}

void CFreeMovement::ProcessMouseScroll(float yoffset)
{
	CCamera *cam = owner->GetComponent<CCamera>();

	if (cam == nullptr)
	{
		return;
	}

	float fov = cam->GetFOV();

	if (fov >= 1.0f && fov <= 120.0f)
		fov -= yoffset * SCROLL_SPEED * Time::GetDeltaTime();
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 120.0f)
		fov = 120.0f;

	cam->SetFOV(fov);
}

void CFreeMovement::FixedUpdate()
{
	ProcessKeyboard();
}
