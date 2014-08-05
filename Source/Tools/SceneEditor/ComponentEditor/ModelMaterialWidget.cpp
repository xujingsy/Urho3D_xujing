#include "stdafx.h"
#include "ModelMaterialWidget.h"
#include <QHBoxLayout>

ModelMaterialWidget::ModelMaterialWidget(QWidget* parent) : QFrame(parent)
{
	vLayout = new QVBoxLayout();

	setLayout(vLayout);
}

ModelMaterialWidget::~ModelMaterialWidget()
{

}

void ModelMaterialWidget::InitComponent(Component* pComponent)
{

}

void ModelMaterialWidget::ClearItems()
{
	QList<QWidget*> lstWidgets = vLayout->findChildren<QWidget*>();
	
	for(int i = 0;i < lstWidgets.size();i ++)
	{
		vLayout->removeWidget(lstWidgets[i]);
	}
}

void ModelMaterialWidget::SetOwnerModel(StaticModel* pModel)
{
	ClearItems();

	const Vector<SourceBatch>& materials = pModel->GetBatches();

	for(int i = 0;i < materials.Size();i ++)
	{
		//添加一条记录
		QHBoxLayout* hRow = new QHBoxLayout();

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

		vLayout->addLayout(hRow);
	}
}

void ModelMaterialWidget::onButtonSelectClicked(bool checked)
{
	QPushButton* btn = (QPushButton*)sender();
	//QString tag = btn->getWhatsThis();

}
