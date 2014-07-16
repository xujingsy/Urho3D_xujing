#include "UIButton.h"
#include "UIElement.h"
#include "ResourceCache.h"
#include "Font.h"
#include "UI.h"
#include "Texture2D.h"
#include "Context.h"

UIButton::UIButton(Context* context) : Button(context_)
{
	ResourceCache* cache = context->GetSubsystem<ResourceCache>();
	Font* font = cache->GetResource<Font>("Fonts/Anonymous Pro.ttf");

	SetStyleAuto();

	text_ = CreateChild<Text>();
	text_->SetColor(Color::BLUE);
	text_->SetAlignment(HA_CENTER, VA_CENTER);

	//SubscribeToEvent(E_MOUSEMOVE,)
}

UIButton::~UIButton()
{

}

void UIButton::RegisterObject(Context* context)
{
	extern const char* UI_CATEGORY;
	context->RegisterFactory<UIButton>("UI");
}

void UIButton::SetText(const String& text)
{
	text_->SetText(text);
}

void UIButton::SetImage(const String& imagePath)
{
	ResourceCache* cache = GetSubsystem<ResourceCache>();
	SetTexture(cache->GetResource<Texture2D>(imagePath));
}
