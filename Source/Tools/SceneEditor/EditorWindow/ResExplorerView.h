#pragma once
#include <QWidget>
#include <QTreeWidget>
#include <QEvent>
#include <QListWidget>
#include <QDirModel>

//目录资源面板
class ResExplorerView : public QWidget
{
	Q_OBJECT
public:
	ResExplorerView(QWidget* parent = 0);
	~ResExplorerView();

public Q_SLOTS:
	void onDirChanged();
private:
	QTreeWidget* mDirWidget;
	QListWidget* mFilesWidget;
};
