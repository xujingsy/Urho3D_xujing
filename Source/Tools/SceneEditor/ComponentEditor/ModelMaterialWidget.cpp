#include "stdafx.h"
#include "ModelMaterialWidget.h"
#include <QHBoxLayout>

ModelMaterialWidget::ModelMaterialWidget()
{
	SetIcon(":/Images/MaterialView/material.png");
	SetName("Materials");

}

ModelMaterialWidget::~ModelMaterialWidget()
{

}

void ModelMaterialWidget::Init(Node* pNode)
{

}

void ModelMaterialWidget::ClearItems()
{
	QList<QWidget*> lstWidgets = frameLayout_->findChildren<QWidget*>();
	
	for(int i = 0;i < lstWidgets.size();i ++)
	{
		frameLayout_->removeWidget(lstWidgets[i]);
	}

	//removeWidget();
}

void ModelMaterialWidget::SetOwnerModel(StaticModel* pModel)
{
	ClearItems();

	const Vector<SourceBatch>& materials = pModel->GetBatches();

	for(int i = 0;i < materials.Size();i ++)
	{
		//添加一条记录
		QHBoxLayout* hRow = getNewRow();

		QLabel* lblMatPath = new QLabel("");
		lblMatPath->setFixedWidth(160);
		hRow->addWidget(lblMatPath);

		QPushButton* btnSelect = new QPushButton("Select");
		hRow->addWidget(btnSelect);
		btnSelect->setWhatsThis(QString::number(i));
		connect(btnSelect, SIGNAL(clicked(bool)), this, SLOT(onButtonSelectClicked(bool)));

		//如果材质为空，
		Material* pMaterial = pModel->GetMaterial(i);
		if(pModel != NULL)
		{
			lblMatPath->setText(pMaterial->GetName().CString());
		}
	}
}

void ModelMaterialWidget::onButtonSelectClicked(bool checked)
{
	QPushButton* btn = (QPushButton*)sender();
	//QString tag = btn->getWhatsThis();

}
