#pragma once
#include "BaseEditorPage.h"

class StaticModelWidget : public BaseEditorPage
{
	Q_OBJECT
public:
	StaticModelWidget();
	void Init(Node* pNode);
public:
	bool CastShadow;
};
