#pragma once
#include "Button.h"
#include "LineEdit.h"
#include "Texture.h"
#include "SpriteSheet2D.h"
#include "BorderImage.h"
#include "Text.h"
#include "ToolTip.h"

namespace Urho3D
{
	class URHO3D_API UIButton : public Button
	{
		OBJECT(UIButton);
	public:
		UIButton(Context* context, const String& text = "");
		virtual ~UIButton();

		static void RegisterObject(Context* context);

		static void SetDefaultSkin(SpriteSheet2D* texSheet, const String& normal, const String& hover, const String& down, const String& disable);

		void SetSkin(SpriteSheet2D* texSheet, const String& buttonName);

		void SetText(const String& text);
		void SetToolTip(const String& tip);

		void SetFont(const String& fontName, int size = DEFAULT_FONT_SIZE);
	private:
		LineEdit* buttonText;
		ToolTip* tooltip;
		Text* toolTipText;

		SpriteSheet2D* skinSheet;
		Sprite2D* tNormal;
		Sprite2D* tHover;
		Sprite2D* tDown;
	};
}
