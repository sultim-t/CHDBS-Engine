#pragma once
#include <Engine\Components\IComponent.h>

class CFreeMovement : public IComponent
{
	CLASSDECLARATION(CFreeMovement)
	
private:
	float x, y;
	float speed;

private:
	void ProcessMouseMovement(float xoffset, float yoffset);
	void ProcessKeyboard();
	void ProcessMouseScroll(float yoffset);
	
	void FixedUpdate(int a);

public:
	void Init() override;
	void Update() override;

	void SetProperty(const String &key, const String &value) override;
};