#include "stdafx.h"
#include "FloatEditor.h"
#include "Vector3Editor.h"

Vector3Editor::Vector3Editor(const QString& text) :
	QGroupBox(text)
{
	QVBoxLayout* vBoxLayout = new QVBoxLayout();
	setLayout(vBoxLayout);

	xEditor = new FloatEditor("X", false);
	vBoxLayout->addLayout(xEditor);
	connect(xEditor, SIGNAL(valueChanged(float)), this, SLOT(editorValueChanged()));

	yEditor = new FloatEditor("Y", false);
	vBoxLayout->addLayout(yEditor);
	connect(yEditor, SIGNAL(valueChanged(float)), this, SLOT(editorValueChanged()));

	zEditor = new FloatEditor("Z", false);
	vBoxLayout->addLayout(zEditor);
	connect(zEditor, SIGNAL(valueChanged(float)), this, SLOT(editorValueChanged()));
}

Vector3Editor::~Vector3Editor()
{

}

void Vector3Editor::setValue(const Vector3& value)
{
	xEditor->setValue(value.x_);
	yEditor->setValue(value.y_);
	zEditor->setValue(value.z_);
}

Vector3 Vector3Editor::value() const
{
	return Vector3(xEditor->value(), yEditor->value(), zEditor->value());
}

void Vector3Editor::setRange(const Vector3& min, const Vector3& max)
{
	xEditor->setRange(min.x_, max.x_);
	yEditor->setRange(min.y_, max.y_);
	zEditor->setRange(min.z_, max.z_);
}

void Vector3Editor::editorValueChanged()
{
	emit valueChanged(value());
}
