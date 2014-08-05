#include "stdafx.h"
#include "MaterialView.h"
#include <QList>
#include <QFont>

MaterialView::MaterialView()
{
	setWindowTitle(tr("Select Material"));
	resize(421, 612);

	materialWidget_ = new QTreeWidget(this);

	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->setMargin(0);
	layout->addWidget(materialWidget_);

	QHBoxLayout* hDetailLayout = new QHBoxLayout();
	QPushButton* btnSelect = new QPushButton("Select");
	hDetailLayout->addWidget(btnSelect);

	layout->addLayout(hDetailLayout);

	connect(materialWidget_, SIGNAL(itemSelectionChanged()), this, SLOT(selectionChanged()));

	//todo:注册文件系统变化事件，重刷列表

	prepareView();
}

MaterialView::~MaterialView()
{

}

void MaterialView::prepareView()
{
	//获得Materials目录文件列表(将来也可以包含其他目录todo)
	Vector<QString> materialNames;
	materialNames.Push("Materials/GreenTransparent.xml");
	materialNames.Push("Materials/Jack.xml");
	materialNames.Push("Materials/Skybox.xml");
	materialNames.Push("Materials/Smoke.xml");
	materialNames.Push("Materials/Stone.xml");

	QTreeWidgetItem* rootItem = new QTreeWidgetItem((QTreeWidgetItem*)NULL, QStringList(tr("Project Materials")));
	rootItem->setIcon(0, QIcon(":/Images/MaterialView/project.png"));

	QFont fnt = rootItem->font(0);
	fnt.setBold(true);
	rootItem->setFont(0, fnt);

	QIcon materialIcon(":/Images/MaterialView/material.png");
	QIcon techIcon(":/Images/MaterialView/technique.png");
	QIcon passIcon(":/Images/MaterialView/pass.png");

	ResourceCache* cache = EditorRoot::Instance()->context_->GetSubsystem<ResourceCache>();

	for(int i = 0;i < materialNames.Size();i ++)
	{
		const QString& materialPath = materialNames[i];
		Material* pMaterial = cache->GetResource<Material>(materialPath.toStdString().c_str());
		if(pMaterial == NULL)
			continue;

		QTreeWidgetItem* matItem = new QTreeWidgetItem((QTreeWidget*)0, QStringList(materialPath));
		matItem->setIcon(0, materialIcon);
		rootItem->addChild(matItem);

		for(int t = 0;t < pMaterial->GetNumTechniques();t ++)
		{
			Technique* tech = pMaterial->GetTechnique(t);
			QTreeWidgetItem* techItem = new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString("Tech") + t));
			techItem->setIcon(0, techIcon);

			matItem->addChild(techItem);
		}
	}

	materialWidget_->addTopLevelItem(rootItem);
}

void MaterialView::selectionChanged()
{
	QList<QTreeWidgetItem*> list = materialWidget_->selectedItems();
	if(list.size() == 0)
	{
		return;
	}

	QTreeWidgetItem* selItem = list[0];
}