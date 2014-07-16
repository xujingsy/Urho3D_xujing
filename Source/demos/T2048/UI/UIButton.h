#pragma once
#include "Button.h"
#include "Text.h"
using namespace Urho3D;

class UIButton : public Button
{
	OBJECT(UIButton);
public:
	UIButton(Context* context);

	~UIButton();

	static void RegisterObject(Context* context);

	void SetText(const String& text);
	void SetImage(const String& imagePath);
private:
	Text* text_;
};
