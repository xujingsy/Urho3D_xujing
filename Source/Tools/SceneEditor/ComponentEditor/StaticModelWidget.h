#pragma once
#include "BaseComponentEditor.h"
#include "ModelMaterialWidget.h"

class StaticModelWidget : public BaseComponentEditor
{
	Q_OBJECT
public:
	StaticModelWidget();

	~StaticModelWidget()
	{
	}

	void InitComponent(Component* pComponent);
public:
	bool CastShadow;
private:
	ModelMaterialWidget* materialEditor_;
};
