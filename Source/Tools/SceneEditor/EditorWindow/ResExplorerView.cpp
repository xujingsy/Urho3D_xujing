#include "stdafx.h"
#include "ResExplorerView.h"
#include <qdirmodel.h>
#include <qtreeview.h>
#include <qboxlayout.h>

ResExplorerView::ResExplorerView(QWidget* parent)
{
	//设置树显示的列数
	setColumnCount(1);
	setHeaderHidden(true);
	setSelectionMode(QAbstractItemView::ExtendedSelection);
	setAutoScroll(true);

	setAcceptDrops(true);
	//预制的几个节点
	//mModelsItem = new QTreeWidgetItem(QStringList("Models"));
	//mModelsItem->setIcon(0,QIcon("Images/paint.svg"));
	//addTopLevelItem(mModelsItem);

	QBoxLayout* layout = new QBoxLayout(QBoxLayout::TopToBottom);
	this->setLayout(layout);

	//test
	QDirModel* model = new QDirModel();
	QTreeView* tree = new QTreeView();
	tree->setModel(model);
	tree->setRootIndex(model->index(QDir::currentPath() + "/Data"));
	tree->setAcceptDrops(true);

	layout->addWidget(tree);
}
