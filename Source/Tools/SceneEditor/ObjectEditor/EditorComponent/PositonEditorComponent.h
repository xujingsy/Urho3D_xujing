#pragma once
#include <qwidget.h>
#include "../BaseEditorPage.h"
#include "../../Controls/LabelTextbox.h"

class PositonEditorComponent : public BaseEditorPage
{
	Q_OBJECT
public:
	PositonEditorComponent(QWidget* parent = 0);
	~PositonEditorComponent();

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
};
