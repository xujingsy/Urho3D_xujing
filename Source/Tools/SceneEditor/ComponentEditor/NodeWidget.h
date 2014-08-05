#pragma once
#include <qwidget.h>
#include "BaseComponentEditor.h"
#include "../Controls/LabelTextbox.h"

class NodeWidget : public BaseComponentEditor
{
	Q_OBJECT
public:
	NodeWidget(QWidget* parent = 0);
	~NodeWidget();

	void Init(Node* pNode);
private:
	LabelTextBox* lblX;
	LabelTextBox* lblY;
	LabelTextBox* lblZ;

	LabelTextBox* lblRX;
	LabelTextBox* lblRY;
	LabelTextBox* lblRZ;

	LabelTextBox* lblSX;
	LabelTextBox* lblSY;
	LabelTextBox* lblSZ;
private slots:
	void valueChanged(float fValue);
	void onEditEnd(float fValue);
};
