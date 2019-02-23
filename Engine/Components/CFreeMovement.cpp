#include "CFreeMovement.h"
#include <Engine/Input/Input.h>
#include <Engine/Entities/Entity.h>
#include <Engine/Components/CCamera.h>

#define MOVE_FORWARD	0
#define MOVE_BACK		1
#define MOVE_LEFT		2
#define MOVE_RIGHT		3

#define SCROLL_SPEED	20

#define PROPERTY_KEY_SPEED	"speed"


CLASSDEFINITION(IComponent, CFreeMovement)

void CFreeMovement::Init()
{ }

void CFreeMovement::Update()
{
	if (Input::IsPressed(Keycode::KeyESCAPE))
	{
		ContextWindow::Instance().RequestClose();
	}

	float x1 = Input::IsPressed(Keycode::KeyRIGHT) ? 1.0f : 0.0f;
	x1 += Input::IsPressed(Keycode::KeyLEFT) ? -1.0f : 0.0f;
	float y1 = Input::IsPressed(Keycode::KeyUP) ? 1.0f : 0.0f;
	y1 += Input::IsPressed(Keycode::KeyDOWN) ? -1.0f : 0.0f;

	float scroll = Input::IsPressed(Keycode::KeyKPSUBTRACT)? -1.0f : 0.0f;
	scroll += Input::IsPressed(Keycode::KeyKPADD)? 1.0f : 0.0f;

	ProcessMouseMovement(x1, y1);
	ProcessMouseScroll(scroll);
	ProcessKeyboard();
}

void CFreeMovement::SetProperty(const String &key, const String &value)
{
	if (key == PROPERTY_KEY_SPEED)
	{
		Speed = value.ToFloat();
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
	Vector3 up = t.GetUp();

	float velocity = Speed * Time::GetDeltaTime();
	
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

	t.Translate(offset);
}

void CFreeMovement::ProcessMouseMovement(float xoffset, float yoffset)
{
	xoffset *= Input::MouseSensitivity * 50;
	yoffset *= Input::MouseSensitivity * 50;

	float x = xoffset * Time::GetDeltaTime();
	float y = yoffset * Time::GetDeltaTime();

	Euler e;
	e[YAW] = x;
	e[PITCH] = y;
	e[ROLL] = 0.0f;

	owner->GetTransform().Rotate(e);
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