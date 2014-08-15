#include "Precompiled.h"
#include "UIButton.h"
#include "Rect.h"
#include "Sprite2D.h"
#include "Texture2D.h"
#include "Context.h"

namespace Urho3D
{
	// default Skin
	static SpriteSheet2D* skinSheetDefault = NULL;
	static Sprite2D* tNormalDefault = NULL;
	static Sprite2D* tHoverDefault = NULL;
	static Sprite2D* tDownDefault = NULL;
	static Sprite2D* tDisable = NULL;

	UIButton::UIButton(Context* context, const String& text) :
		Button(context)
	{
		buttonText = NULL;
		tooltip = NULL;
		toolTipText = NULL;

		SetBlendMode(BLEND_ADD);

		if(skinSheetDefault)
		{
			SetTexture(skinSheetDefault->GetTexture());

			//1.Normal
			SetImageRect(tNormalDefault->GetRectangle());

			//2.Hover
			SetHoverOffset(tHoverDefault->GetRectangle().left_ - tNormalDefault->GetRectangle().left_, tHoverDefault->GetRectangle().top_ - tNormalDefault->GetRectangle().top_);

			//3.Pressed
			SetPressedOffset(tDownDefault->GetRectangle().left_  - tNormalDefault->GetRectangle().left_, tDownDefault->GetRectangle().top_  - tNormalDefault->GetRectangle().top_);
		}

		//Default Size
		const IntRect& rect = tNormalDefault->GetRectangle();
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
			buttonText = CreateChild<LineEdit>();
			buttonText->SetColor(Color::RED);
			buttonText->SetStyleAuto();
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
		skinSheetDefault = texSheet;

		tNormalDefault = texSheet->GetSprite(normal);
		tHoverDefault = texSheet->GetSprite(hover);
		tDownDefault = texSheet->GetSprite(down);
	}

	/// buttonName0,buttonName1,buttonName2,buttonNam3
	void UIButton::SetSkin(SpriteSheet2D* texSheet, const String& buttonName)
	{
		skinSheet = texSheet;
		tNormal = skinSheet->GetSprite(buttonName + "0");
		tHover = skinSheet->GetSprite(buttonName + "1");
		tDown = skinSheet->GetSprite(buttonName + "2");
	}
}
