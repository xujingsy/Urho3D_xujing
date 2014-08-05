#include "stdafx.h"
#include "AnimatedModelWidget.h"

AnimatedModelWidget::AnimatedModelWidget()
{
	SetIcon(":/Images/Components/AnimatedModel.png");
	SetName("AnimatedModel");

	boneWidget_ = new QTreeWidget(this);
	frameLayout_->addWidget(boneWidget_);
}

void AnimatedModelWidget::InitComponent(Component* pComponent)
{

}
