#pragma once
#include "BaseComponentEditor.h"
#include <QCheckBox>

//SceneRoot节点信息显示
class SceneWidget : public BaseComponentEditor
{
	Q_OBJECT
public:
	SceneWidget();

	void Init(Node* pNode);
private Q_SLOTS:
	void on_physics_click(int state);
	void on_debugrender_click(int state);
private:
	QCheckBox* chkOctree;
	QCheckBox* chkPhysics;
	QCheckBox* chkDebugRender;
};
