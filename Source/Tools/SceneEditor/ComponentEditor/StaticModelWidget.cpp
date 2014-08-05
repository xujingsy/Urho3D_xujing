#include "stdafx.h"
#include "StaticModelWidget.h"

StaticModelWidget::StaticModelWidget()
{
	CastShadow = true;
	QBoxLayout* layout = new QBoxLayout(QBoxLayout::TopToBottom);

	SetIcon(":/Images/Components/StaticModel.png");
	SetName("StaticModel");

	materialEditor_ = new ModelMaterialWidget();
	frameLayout_->addWidget(materialEditor_);
}

void StaticModelWidget::InitComponent(Component* pComponent)
{

}
