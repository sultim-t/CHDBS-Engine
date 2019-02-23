#pragma once
#include <Engine\Components\IComponent.h>

class CFreeMovement : public IComponent
{
	CLASSDECLARATION(CFreeMovement)
	
public:
	float x, y;
	float Speed;

private:
	void ProcessMouseMovement(float xoffset, float yoffset);
	void ProcessKeyboard();
	void ProcessMouseScroll(float yoffset);

public:
	void Init() override;
	void Update() override;

	void SetProperty(const String &key, const String &value) override;
};