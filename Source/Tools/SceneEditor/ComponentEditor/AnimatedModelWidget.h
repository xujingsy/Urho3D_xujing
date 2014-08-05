#pragma once
#include "BaseComponentEditor.h"
#include <QTreeWidget>

class AnimatedModelWidget : public BaseComponentEditor
{
	Q_OBJECT
public:
	AnimatedModelWidget();

	void InitComponent(Component* pComponent);
private:
	void showBones();

	QTreeWidget* boneWidget_;
};
