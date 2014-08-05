#pragma once
#include <QWidget>
#include <QTreeWidget>
#include <QDialog>

//当点击某个Index 的Material时弹出该面板，选择Material
class MaterialView : public QDialog
{
	Q_OBJECT;
public:
	MaterialView();
	~MaterialView();

	//初始化材质列表
	void prepareView();
private slots:
	//Tree中选择了不同的材质
	void selectionChanged();
private:
	QTreeWidget* materialWidget_;
};
