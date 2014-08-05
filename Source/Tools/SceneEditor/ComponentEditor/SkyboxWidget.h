#pragma once
#include "StaticModelWidget.h"

class SkyboxWidget : public StaticModelWidget
{
	Q_OBJECT
public:
	SkyboxWidget()
	{
	}

	void Init(Node* pNode);
};
