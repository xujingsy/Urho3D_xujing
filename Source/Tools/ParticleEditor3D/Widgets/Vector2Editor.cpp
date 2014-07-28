#include "stdafx.h"
#include "FloatEditor.h"
#include "Vector2Editor.h"

Vector2Editor::Vector2Editor(const QString& text) :
    QGroupBox(text)
{
    QVBoxLayout* vBoxLayout = new QVBoxLayout();
    setLayout(vBoxLayout);

    xEditor_ = new FloatEditor("X", false);
    vBoxLayout->addLayout(xEditor_);
    connect(xEditor_, SIGNAL(valueChanged(float)), this, SLOT(editorValueChanged()));

    yEditor_ = new FloatEditor("Y", false);
    vBoxLayout->addLayout(yEditor_);
    connect(yEditor_, SIGNAL(valueChanged(float)), this, SLOT(editorValueChanged()));
}

Vector2Editor::~Vector2Editor()
{
    delete xEditor_;
    delete yEditor_;
}

void Vector2Editor::setValue(const Vector2& value)
{
    xEditor_->setValue(value.x_);
    yEditor_->setValue(value.y_);
}

Vector2 Vector2Editor::value() const
{
    return Vector2(xEditor_->value(), yEditor_->value());
}

void Vector2Editor::setRange(const Vector2& min, const Vector2& max)
{
    xEditor_->setRange(min.x_, max.x_);
    yEditor_->setRange(min.y_, max.y_);
}

void Vector2Editor::editorValueChanged()
{
    emit valueChanged(value());
}
