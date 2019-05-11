#include "UIText.h"
#include "TextRenderer.h"

UIText::UIText(const char * name) : UIElement(name)
{ }

const String & UIText::GetFontPath() const
{
	return font;
}

void UIText::BindFont(const char * fontPath)
{
	this->font = fontPath;
	
	// temp singleton
	// temp loading font here
	TextRenderer::Instance().Init(fontPath);
}

int UIText::GetFontSize() const
{
	return fontSize;
}

void UIText::SetFontSize(int size)
{
	this->fontSize = size;
}

void UIText::Draw()
{
	const Vector2 &pos = GetTransform().GetPosition();
	const Vector2 &scale = GetTransform().GetScale();

	TextRenderer::Instance().Draw(Text, pos[0], pos[1], scale[0], scale[1], Color);
}
