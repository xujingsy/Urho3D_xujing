#pragma once

#include "Vector2.h"
using namespace Urho3D;
#include <QGroupBox>

class FloatEditor;

class ValueVarianceEditor : public QGroupBox
{
    Q_OBJECT

public:
    ValueVarianceEditor(const QString& text);
    virtual ~ValueVarianceEditor();

public:
    /// Set value.
    void setValue(float avarage, float variance);
    void setRange(float min, float max);

    float value() const;
    float variance() const;

signals:
    void valueChanged(float average, float variance);

protected slots:
    void editorValueChanged();

private:
    FloatEditor* averageEditor_;
    FloatEditor* varianceEditor_;
};
