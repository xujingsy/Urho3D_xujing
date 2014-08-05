#pragma once
#include "BaseComponentEditor.h"

class ScriptInstanceWidget : public BaseComponentEditor
{
	Q_OBJECT
public:
	ScriptInstanceWidget()
	{
	}

	void Init(Node* pNode);
};
