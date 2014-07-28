#pragma once

#include "Vector2.h"
using namespace Urho3D;
#include <QGroupBox>

class FloatEditor;

class Vector2Editor : public QGroupBox
{
    Q_OBJECT

public:
    Vector2Editor(const QString& text);
    virtual ~Vector2Editor();

public:
    /// Set value.
    void setValue(const Vector2& value);
    /// Return value.
    Vector2 value() const;

    /// Set range.
    void setRange(const Vector2& min, const Vector2& max);

signals:
    void valueChanged(const Vector2&);

protected slots:
    void editorValueChanged();

private:
    FloatEditor* xEditor_;
    FloatEditor* yEditor_;
};
