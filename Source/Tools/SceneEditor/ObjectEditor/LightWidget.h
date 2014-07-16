#pragma once
#include "BaseEditorPage.h"
#include "../Controls/ColorPickerWidget.h"
#include "../Controls/LabelTextbox.h"

class LightWidget : public BaseEditorPage
{
	Q_OBJECT
public:
	LightWidget(QWidget* parent = 0);

	void Init(Node* pNode);
private slots:
	void onLightTypeChanged(int index);
	void onLightColorChanged(Urho3D::Color color);
	void onLightSpeckIntenChanged(float intensity);
private:
	QComboBox* cboLightType;
	ColorPickerWidget* selColor;
	LabelTextBox* lblSpeInden;	//反光强度
};
