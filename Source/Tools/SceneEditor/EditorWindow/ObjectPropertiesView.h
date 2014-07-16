#pragma once
#include <list>
using namespace std;
#include <qwidget.h>
#include "../Controls/LabelTextbox.h"
#include "../ObjectEditor/EditorComponent/PositonEditorComponent.h"
#include "../Controls/ShowHidePanel.h"
#include "../ObjectEditor/LightWidget.h"
#include "../ObjectEditor/SceneWidget.h"
#include "../ObjectEditor/SkyboxWidget.h"
#include "../ObjectEditor/StaticModelWidget.h"
#include "../ObjectEditor/TerrainWidget.h"

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
	SkyboxWidget* skyboxWidget_;
	StaticModelWidget* staticModelWidget_;
	TerrainWidget* terrainWidget_;

	list<ShowHidePanel*> lstWidges;

	QWidget* fixWidget;
};
