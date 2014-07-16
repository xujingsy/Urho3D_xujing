#include "stdafx.h"
#include "LightWidget.h"

LightWidget::LightWidget(QWidget* parent)
{
	SetIcon("Images/light.png");
	SetName("Light");

	cboLightType = new QComboBox();
	//indexÓëLightType¶ÔÓ¦
	cboLightType->addItem("Directional");
	cboLightType->addItem("Spot");
	cboLightType->addItem("Point");
	frameLayout_->addWidget(cboLightType);
	connect(cboLightType,SIGNAL(currentIndexChanged(int)),this,SLOT(onLightTypeChanged(int)));

	QHBoxLayout* colorSelect = getNewRow();
	QLabel* lblColor = new QLabel("Color");
	colorSelect->addWidget(lblColor);
	selColor = new ColorPickerWidget(NULL,QColor(0,0,0));
	colorSelect->addWidget(selColor);
	connect(selColor,SIGNAL(colorChanged(Urho3D::Color)),this,SLOT(onLightColorChanged(Urho3D::Color)));

	QHBoxLayout* rowSpec = getNewRow();
	lblSpeInden = new LabelTextBox();
	lblSpeInden->SetName("Specular Itensity");
	lblSpeInden->SetValueRange(0,5);
	rowSpec->addWidget(lblSpeInden);
	connect(lblSpeInden,SIGNAL(valueChanged(float)),this,SLOT(onLightSpeckIntenChanged(float)));
}

void LightWidget::onLightTypeChanged(int index)
{
	Light* light = pOwnerNode->GetComponent<Light>();
	if(light)
	{
		if(index >= 0)
		{
			light->SetLightType(LightType(index));
		}
	}
}

void LightWidget::onLightColorChanged(Urho3D::Color color)
{
	Light* pLight = pOwnerNode->GetComponent<Light>();
	if(pLight != NULL)
	{
		pLight->SetColor(color);
	}
}

void LightWidget::onLightSpeckIntenChanged(float intensity)
{
	Light* pLight = pOwnerNode->GetComponent<Light>();
	if(pLight != NULL)
	{
		pLight->SetSpecularIntensity(intensity);
	}
}

void LightWidget::Init(Node* pNode)
{
	pOwnerNode = pNode;

	BeginInit();

	Light* pLight = pOwnerNode->GetComponent<Light>();
	if(pLight != NULL)
	{
		selColor->setColor(pLight->GetColor());
		lblSpeInden->SetText(QString::number(pLight->GetSpecularIntensity()).toStdString().c_str());
		cboLightType->setCurrentIndex((int)pLight->GetLightType());
	}

	EndInit();
}