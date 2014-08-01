#include "stdafx.h"
#include "ResExplorerView.h"

ResExplorerView::ResExplorerView(QWidget* parent)
{
	QHBoxLayout* layout = new QHBoxLayout();

	QDirModel* model = new QDirModel();
	model->setFilter(QDir::AllDirs);

	mDirWidget = new QTreeWidget();
	//mDirWidget->setModel(model);
	//mDirWidget->setColumnHidden(1, true);
	//mDirWidget->setColumnHidden(2, true);
	//mDirWidget->setColumnHidden(3, true);

	mDirWidget->setRootIndex(model->index(QDir::currentPath() + "/Data"));
	mDirWidget->setAcceptDrops(true);

	layout->addWidget(mDirWidget);

	connect(mDirWidget, SIGNAL(itemSelectionChanged()), this, SLOT(onDirChanged()));

	//设置树显示的列数
	//mDirWidget->setColumnCount(1);
	mDirWidget->setHeaderHidden(true);
	mDirWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
	mDirWidget->setAutoScroll(true);
	
	mFilesWidget = new QListWidget();
	setAcceptDrops(true);

	layout->addWidget(mFilesWidget);

	this->setLayout(layout);
}

ResExplorerView::~ResExplorerView()
{

}

void ResExplorerView::onDirChanged()
{

}
