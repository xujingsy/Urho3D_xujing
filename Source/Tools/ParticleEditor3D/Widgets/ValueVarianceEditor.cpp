#include "stdafx.h"
#include "FloatEditor.h"
#include "ValueVarianceEditor.h"
    
ValueVarianceEditor::ValueVarianceEditor(const QString& text) :
    QGroupBox(text)
{
    QVBoxLayout* vBoxLayout = new QVBoxLayout();
    setLayout(vBoxLayout);

    averageEditor_ = new FloatEditor(tr("Value"), false);
    vBoxLayout->addLayout(averageEditor_);

    averageEditor_->label()->setFixedWidth(64);
    connect(averageEditor_, SIGNAL(valueChanged(float)), this, SLOT(editorValueChanged()));

    varianceEditor_ = new FloatEditor(tr("Variance"), false);
    vBoxLayout->addLayout(varianceEditor_);

    varianceEditor_->label()->setFixedWidth(64);
    connect(varianceEditor_, SIGNAL(valueChanged(float)), this, SLOT(editorValueChanged()));
}

ValueVarianceEditor::~ValueVarianceEditor()
{
    delete averageEditor_;
    delete varianceEditor_;
}

void ValueVarianceEditor::setValue(float avarage, float variance)
{
    averageEditor_->setValue(avarage);
    varianceEditor_->setValue(variance);
}


void ValueVarianceEditor::editorValueChanged()
{
    emit valueChanged(value(), variance());
}

void ValueVarianceEditor::setRange(float min, float max)
{
    averageEditor_->setRange(min, max);
    varianceEditor_->setRange(0.0f, max - min);
}

float ValueVarianceEditor::value() const
{
    return averageEditor_->value();
}

float ValueVarianceEditor::variance() const
{
    return varianceEditor_->value();
}
