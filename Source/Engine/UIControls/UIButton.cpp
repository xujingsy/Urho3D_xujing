#include "Precompiled.h"
#include "UIButton.h"
#include "Rect.h"
#include "Sprite2D.h"
#include "Texture2D.h"
#include "Context.h"

namespace Urho3D
{
	// default Skin
	static SpriteSheet2D* skinSheet = NULL;
	static Sprite2D* tNormal = NULL;
	static Sprite2D* tHover = NULL;
	static Sprite2D* tDown = NULL;
	static Sprite2D* tDisable = NULL;

	UIButton::UIButton(Context* context, const String& text) :
		Button(context)
	{
		buttonText = NULL;
		tooltip = NULL;
		toolTipText = NULL;

		SetBlendMode(BLEND_ADD);

		SetTexture(skinSheet->GetTexture());

		//1.Normal
		SetImageRect(tNormal->GetRectangle());

		//2.Hover
		SetHoverOffset(tHover->GetRectangle().left_ - tNormal->GetRectangle().left_, tHover->GetRectangle().top_ - tNormal->GetRectangle().top_);

		//3.Pressed
		SetPressedOffset(tDown->GetRectangle().left_  - tNormal->GetRectangle().left_, tDown->GetRectangle().top_  - tNormal->GetRectangle().top_);

		//Default Size
		const IntRect& rect = tNormal->GetRectangle();
		SetSize(rect.Width(), rect.Height());

		if(text != "")
		{
			SetText(text);
		}
	}

	UIButton::~UIButton()
	{

	}

	extern const char* UI_CATEGORY;
	void UIButton::RegisterObject(Context* context)
	{
		context->RegisterFactory<UIButton>(UI_CATEGORY);
	}

	void UIButton::SetText(const String& text)
	{
		if(buttonText == NULL)
		{
			buttonText = new LineEdit(context_);
			this->AddChild(buttonText);
		}

		buttonText->SetText(text);
	}

	void UIButton::SetToolTip(const String& tip)
	{
		if(tooltip == NULL)
		{
			tooltip = new ToolTip(context_);
			AddChild(tooltip);
			tooltip->SetPosition(IntVector2(GetWidth() + 5, GetWidth() / 2)); // slightly offset from close button

			BorderImage* textHolder = new BorderImage(context_);
			tooltip->AddChild(textHolder);
			textHolder->SetStyle("ToolTipBorderImage");

			toolTipText = new Text(context_);
			textHolder->AddChild(toolTipText);
			toolTipText->SetStyle("ToolTipText");
		}

		toolTipText->SetText(tip);
	}

	void UIButton::SetDefaultSkin(SpriteSheet2D* texSheet, const String& normal, const String& hover, const String& down, const String& disable)
	{
		skinSheet = texSheet;

		tNormal = texSheet->GetSprite(normal);
		tHover = texSheet->GetSprite(hover);
		tDown = texSheet->GetSprite(down);
	}

}
