#include "CFreeMovement.h"
#include <Engine/Input/Input.h>
#include <Engine/Entities/Entity.h>
#include <Engine/Components/CCamera.h>
#include <Engine/Base/String.h>

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
	if (Input::IsPressed(INPUT_KEY_ESCAPE))
	{
		ContextWindow::Instance().RequestClose();
	}

	float y1 = 0;
	float x1 = Input::IsPressed(INPUT_KEY_RIGHT) ? 1.0f : 0.0f;
	x1 += Input::IsPressed(INPUT_KEY_LEFT) ? -1.0f : 0.0f;
	if (x1 == 0)
	{
		y1 = Input::IsPressed(INPUT_KEY_UP) ? 1.0f : 0.0f;
		y1 += Input::IsPressed(INPUT_KEY_DOWN) ? -1.0f : 0.0f;
	}

	float scroll = Input::IsPressed(INPUT_KEY_KP_SUBTRACT)? -1.0f : 0.0f;
	scroll += Input::IsPressed(INPUT_KEY_KP_ADD)? 1.0f : 0.0f;

	ProcessMouseMovement(x1, y1);
	ProcessMouseScroll(scroll);
	ProcessKeyboard();
}

void CFreeMovement::SetProperty(const String & key, const String & value)
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

	float velocity = Speed * Time::GetDeltaTime();
	
	if (Input::IsPressed(INPUT_KEY_W))
		offset += front * velocity;
	
	if (Input::IsPressed(INPUT_KEY_S))
		offset -= front * velocity;
	
	if (Input::IsPressed(INPUT_KEY_A))
		offset -= right * velocity;

	if (Input::IsPressed(INPUT_KEY_D))
		offset += right * velocity;

	t.Translate(offset);
}

void CFreeMovement::ProcessMouseMovement(float xoffset, float yoffset)
{
	xoffset *= Input::MouseSensitivity;
	yoffset *= Input::MouseSensitivity;

	float yaw = xoffset * Time::GetDeltaTime();
	float pitch = yoffset * Time::GetDeltaTime();

	Euler e = Euler(0, pitch, yaw);
	owner->GetTransform().Rotate(e);
}

void CFreeMovement::ProcessMouseScroll(float yoffset)
{
	CCamera *cam = owner->GetComponent<CCamera>();

	float fov = cam->GetFOV();

	if (fov >= 1.0f && fov <= 180.0f)
		fov -= yoffset * SCROLL_SPEED * Time::GetDeltaTime();
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 180.0f)
		fov = 180.0f;

	cam->SetFOV(fov);
}