#include "stdafx.h"
#include "PositonEditorComponent.h"

PositonEditorComponent::PositonEditorComponent(QWidget* parent)
{
	pOwnerNode = NULL;

	SetIcon(":/Images/Coord.png");
	SetName("Transform");

	//1.Position
	QLabel* lblPosition = new QLabel("Position");
	frameLayout_->addWidget(lblPosition);

	QHBoxLayout* rowPosition = getNewRow();
	lblX = new LabelTextBox();
	lblX->SetValueRange(-9999,9999);
	lblX->SetName("X");
	rowPosition->addWidget(lblX);

	lblY = new LabelTextBox();
	lblY->SetValueRange(-9999,9999);
	lblY->SetName("Y");
	rowPosition->addWidget(lblY);

	lblZ = new LabelTextBox();
	lblZ->SetValueRange(-9999,9999);
	lblZ->SetName("Z");
	rowPosition->addWidget(lblZ);

	//2.Rotation
	QLabel* lblRotation = new QLabel("Rotation");
	frameLayout_->addWidget(lblRotation);
	QHBoxLayout* rowRotation = getNewRow();
	lblRX = new LabelTextBox();
	lblRX->SetValueRange(-90,90);
	lblRX->SetName("X");
	rowRotation->addWidget(lblRX);

	lblRY = new LabelTextBox();
	lblRY->SetValueRange(-90,90);
	lblRY->SetName("Y");
	rowRotation->addWidget(lblRY);

	lblRZ = new LabelTextBox();
	lblRZ->SetValueRange(-90,90);
	lblRZ->SetName("Z");
	rowRotation->addWidget(lblRZ);

	//3.Scale
	QLabel* lblScale = new QLabel("Scale");
	frameLayout_->addWidget(lblScale);
	QHBoxLayout* rowScale = getNewRow();
	lblSX = new LabelTextBox();
	lblSX->SetValueRange(-1000,1000);
	lblSX->SetName("X");
	rowScale->addWidget(lblSX);

	lblSY = new LabelTextBox();
	lblSY->SetValueRange(-1000,1000);
	lblSY->SetName("Y");
	rowScale->addWidget(lblSY);

	lblSZ = new LabelTextBox();
	lblSZ->SetValueRange(-1000,1000);
	lblSZ->SetName("Z");
	rowScale->addWidget(lblSZ);

	//×¢²áÊÂ¼þ
	connect(lblX,SIGNAL(valueChanged(float)),this,SLOT(valueChanged(float)));
	connect(lblY,SIGNAL(valueChanged(float)),this,SLOT(valueChanged(float)));
	connect(lblZ,SIGNAL(valueChanged(float)),this,SLOT(valueChanged(float)));

	connect(lblRX,SIGNAL(valueChanged(float)),this,SLOT(valueChanged(float)));
	connect(lblRY,SIGNAL(valueChanged(float)),this,SLOT(valueChanged(float)));
	connect(lblRZ,SIGNAL(valueChanged(float)),this,SLOT(valueChanged(float)));

	connect(lblSX,SIGNAL(valueChanged(float)),this,SLOT(valueChanged(float)));
	connect(lblSY,SIGNAL(valueChanged(float)),this,SLOT(valueChanged(float)));
	connect(lblSZ,SIGNAL(valueChanged(float)),this,SLOT(valueChanged(float)));
}

PositonEditorComponent::~PositonEditorComponent()
{

}

void PositonEditorComponent::valueChanged(float fValue)
{
	if(bEditNotify == false)
		return;

	LabelTextBox* lblTxt = (LabelTextBox*)sender();
	if(lblTxt == lblX || lblTxt == lblY || lblTxt == lblZ)
	{
		Vector3 position(lblX->GetText().toFloat(),lblY->GetText().toFloat(),lblZ->GetText().toFloat());
		pOwnerNode->SetPosition(position);
	}
	else if(lblTxt == lblRX || lblTxt == lblRY ||lblTxt == lblRZ)
	{
		Quaternion qua(lblRX->GetText().toFloat(),lblRY->GetText().toFloat(),lblRZ->GetText().toFloat());
		pOwnerNode->SetRotation(qua);
	}
	else if(lblTxt == lblSX || lblTxt == lblSY ||lblTxt == lblSZ)
	{
		Vector3 scale(lblSX->GetText().toFloat(),lblSY->GetText().toFloat(),lblSZ->GetText().toFloat());
		pOwnerNode->SetScale(scale);
	}
}

void PositonEditorComponent::Init(Node* pNode)
{
	pOwnerNode = pNode;

	BeginInit();

	const Vector3& position = pOwnerNode->GetPosition();
	lblX->SetText(QString::number(position.x_).toStdString().c_str());
	lblY->SetText(QString::number(position.y_).toStdString().c_str());
	lblZ->SetText(QString::number(position.z_).toStdString().c_str());

	const Quaternion& qua = pOwnerNode->GetRotation();
	lblRX->SetText(QString::number(qua.x_).toStdString().c_str());
	lblRY->SetText(QString::number(qua.y_).toStdString().c_str());
	lblRZ->SetText(QString::number(qua.z_).toStdString().c_str());

	const Vector3& scale = pOwnerNode->GetScale();
	lblSX->SetText(QString::number(scale.x_).toStdString().c_str());
	lblSY->SetText(QString::number(scale.y_).toStdString().c_str());
	lblSZ->SetText(QString::number(scale.z_).toStdString().c_str());

	EndInit();
}
