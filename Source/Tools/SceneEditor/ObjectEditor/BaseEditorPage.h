#pragma once
#include "../Controls/ShowHidePanel.h"
#include "Node.h"
#include <qwidget.h>
#include <qcombobox.h>
#include <qcheckbox.h>

class BaseEditorPage : public ShowHidePanel
{
public:
	virtual void Init(Node* pNode)
	{
		bEditNotify = true;
	}
protected:
	Node* pOwnerNode;

	//避免互相更新导致的错误
	bool bEditNotify;	//是否可编辑

	void BeginInit()
	{
		bEditNotify = false;
	}

	void EndInit()
	{
		bEditNotify = true;
	}
};