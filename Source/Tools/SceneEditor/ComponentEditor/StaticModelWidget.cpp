#include "stdafx.h"
#include "StaticModelWidget.h"

StaticModelWidget::StaticModelWidget()
{
	CastShadow = true;
	QBoxLayout* layout = new QBoxLayout(QBoxLayout::TopToBottom);

	SetIcon(":/Images/Components/StaticModel.png");
	SetName("StaticModel");

}

void StaticModelWidget::Init(Node* pNode)
{

}
