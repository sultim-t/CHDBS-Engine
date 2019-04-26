#pragma once
#include <Engine/Components/IComponent.h>
#include <Engine/Base/Event.h>

class CFreeMovement : public IComponent, public IEventHandler
{
	CLASSDECLARATION(CFreeMovement)
	
private:
	float x, y;
	float speed;

private:
	void ProcessMouseMovement(float xoffset, float yoffset);
	void ProcessKeyboard();
	void ProcessMouseScroll(float yoffset);
	
	void FixedUpdate();

public:
	void Init() override;
	void Update() override;

	void SetProperty(const String &key, const String &value) override;

public:
	void Function(int a) override;
};