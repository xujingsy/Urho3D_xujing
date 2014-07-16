#pragma once
#include "BaseEditorPage.h"

class SkyboxWidget : public BaseEditorPage
{
	Q_OBJECT
public:
	SkyboxWidget()
	{
	}

	void Init(Node* pNode);
};
