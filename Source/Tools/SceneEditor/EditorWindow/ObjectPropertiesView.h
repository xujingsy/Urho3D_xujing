#pragma once
#include <list>
using namespace std;
#include <QWidget>
#include <QPushButton>
#include "../Controls/LabelTextbox.h"

#include "../Controls/ShowHidePanel.h"
#include "../ComponentEditor/LightWidget.h"
#include "../ComponentEditor/SceneWidget.h"
#include "../ComponentEditor/SkyboxWidget.h"
#include "../ComponentEditor/StaticModelWidget.h"
#include "../ComponentEditor/TerrainWidget.h"
#include "../ComponentEditor/ModelMaterialWidget.h"
#include "../ComponentEditor/EditorComponent/PositonEditorComponent.h"

class ObjectPropertiesView : public QWidget
{
public:
	ObjectPropertiesView(QWidget* parent = NULL);

	void SetTarget(Node* pTarget);
private:
	QWidget* wContent;
	QVBoxLayout* vContentLayout;
	LabelTextBox* descWidget_;
	QVBoxLayout* layoutContent;

	//一些对象编辑面板
	PositonEditorComponent* positionWidget_;
	LightWidget* lightWidget_;
	SceneWidget* sceneWidget_;
	ModelMaterialWidget* materialWidget_;
	SkyboxWidget* skyboxWidget_;
	StaticModelWidget* staticModelWidget_;
	TerrainWidget* terrainWidget_;
	QPushButton* btnAddComponent;

	list<ShowHidePanel*> lstWidges;

	QWidget* fixWidget;
};
