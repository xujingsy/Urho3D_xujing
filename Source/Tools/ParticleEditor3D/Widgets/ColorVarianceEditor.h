#pragma once

#include "Color.h"
using namespace Urho3D;
#include <QGroupBox>

class QComboBox;
class QFrame;

class FloatEditor;

class ColorVarianceEditor : public QGroupBox
{
    Q_OBJECT

public:
    ColorVarianceEditor(const QString& text);
    virtual ~ColorVarianceEditor();

public:
    /// Set value.
    void setValue(const Color& avarage, const Color& variance);
    /// Return value.
    Color avarage() const;
    Color variance() const;

signals:
    void valueChanged(const Color& avarage, const Color& variance);

protected slots:
    void colorModeComboBoxIndexChanged(int index);
    void editorValueChanged();

private:
    FloatEditor* CreateFloatEditor(const QString& text);

    QComboBox* colorModeComboBox_;

    QFrame* frame_;

    QGroupBox* minColorGroupBox_;
    FloatEditor* minREditor_;
    FloatEditor* minGEditor_;
    FloatEditor* minBEditor_;
    FloatEditor* minAEditor_;

    QGroupBox* maxColorGroupBox_;
    FloatEditor* maxREditor_;
    FloatEditor* maxGEditor_;
    FloatEditor* maxBEditor_;
    FloatEditor* maxAEditor_;
};
