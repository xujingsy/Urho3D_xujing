#pragma once
#include <QWidget>
#include <QFrame>
#include <QVBoxLayout>

#include "StaticModel.h"
using namespace Urho3D;

//给模型(StaticModel,AnimationModel等)指定材质的窗口
class ModelMaterialWidget : public QFrame
{
	Q_OBJECT
public:
	ModelMaterialWidget(QWidget* parent = NULL);
	~ModelMaterialWidget();

	void InitComponent(Component* pComponent);

	void SetOwnerModel(StaticModel* pModel);

private Q_SLOTS:
	void onButtonSelectClicked(bool checked);
	void onColorFramesClicked(bool checked);
private:
	//移除所有的条目，根据模型的材质数量动态设置
	void ClearItems();
	QVBoxLayout* vLayout;
};
