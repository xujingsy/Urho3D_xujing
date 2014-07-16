#include "stdafx.h"
#include "RectSelectionFrame.h"
#include <qpainter.h>
#include <qpen.h>
#include <qcolor.h>
#include "../EditorSceneWidget.h"

RectSelectionFrame::RectSelectionFrame()
{
	Context* context_ = EditorsRoot::Instance()->context_;

	border_ = new BorderImage(context_);
	ResourceCache* cache = EditorsRoot::Instance()->engine_->GetContext()->GetSubsystem<ResourceCache>();

	border_->SetTexture(cache->GetResource<Texture2D>("Images/brushes/Rect.png"));
	border_->SetBlendMode(BlendMode::BLEND_SUBTRACT);
	border_->SetPosition(100,100);

	EditorsRoot::Instance()->UIRoot_->AddChild(border_);

	isBegin = false;
}

void RectSelectionFrame::Begin(int x,int y)
{
	vecStart.x_ = x;
	vecStart.y_ = y;

	vecEnd = vecStart;

	isBegin = true;

	border_->SetPosition(x,y);
}

void RectSelectionFrame::UpdateShow(int x,int y)
{
	vecEnd.x_ = x;
	vecEnd.y_ = y;
}

void RectSelectionFrame::Hide()
{
	border_->SetVisible(false);
	isBegin = false;
}
