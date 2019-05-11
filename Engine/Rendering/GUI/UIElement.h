#pragma once
#include <Engine/Math/Transform.h>

enum class AnchorOption
{
	Center,
	TopLeft,
	TopRight,
	Bottomleft,
	BottomRight
};

class UIElement
{
private:
	Transform		transform;
	String			name;

public:
	AnchorOption	Anchor;
	Color4F			Color;
	bool			IsActive;

public:
	UIElement(const char *name);

	virtual void Draw() = 0;

	Transform &GetTransform();
	const Transform &GetTransform() const;
	const String &GetName() const;
};

inline UIElement::UIElement(const char *name) 
	: name(name), IsActive(true), Color(1, 1, 1, 1), Anchor(AnchorOption::Center) { }

inline Transform & UIElement::GetTransform()
{
	return transform;
}

inline const Transform & UIElement::GetTransform() const
{
	return transform;
}

inline const String & UIElement::GetName() const
{
	return name;
}