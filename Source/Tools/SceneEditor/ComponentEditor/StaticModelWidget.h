#pragma once
#include "BaseComponentEditor.h"

class StaticModelWidget : public BaseComponentEditor
{
	Q_OBJECT
public:
	StaticModelWidget();
	void Init(Node* pNode);
public:
	bool CastShadow;
};
