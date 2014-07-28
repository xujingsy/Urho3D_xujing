#include "stdafx.h"
#include "IntEditor.h"

IntEditor::IntEditor(const QString& text) :
    senderIsSlider_(false),
    senderIsSpinBox_(false)
{
    setContentsMargins(0, 0, 0, 0);

    label_ = new QLabel(text);
    addWidget(label_, 0);

    slider_ = new QSlider(Qt::Horizontal);
    addWidget(slider_, 1);
    connect(slider_, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)));

    spinBox_ = new QSpinBox;
    addWidget(spinBox_);
    connect(spinBox_, SIGNAL(valueChanged(int)), this, SLOT(spinBoxValueChanged(int)));
}

IntEditor::~IntEditor()
{
    delete label_;
    delete slider_;
    delete spinBox_;
}

void IntEditor::setValue(int value)
{
    spinBox_->setValue(value);
}

void IntEditor::setRange(int min, int max)
{
    slider_->setRange(min, max);
    spinBox_->setRange(min, max);
}

int IntEditor::value() const
{
    return (int)spinBox_->value();
}

void IntEditor::sliderValueChanged(int value)
{
    senderIsSlider_ = true;

    if (!senderIsSpinBox_)
        spinBox_->setValue(value);

    senderIsSlider_ = false;
}

void IntEditor::spinBoxValueChanged(int value)
{
    senderIsSpinBox_ = true;

    if (!senderIsSlider_)
        slider_->setValue(value);

    senderIsSpinBox_ = false;

    emit valueChanged(value);
}
