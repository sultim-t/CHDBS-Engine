#pragma once
#include "UIElement.h"

class UIText : public UIElement
{
private:
	String font;
	int fontSize;

public:
	String Text;

public:
	UIText(const char *name);

	virtual void Draw() override;

	const String &GetFontPath() const;
	void BindFont(const char *fontPath);

	int GetFontSize() const;
	void SetFontSize(int size);
};
