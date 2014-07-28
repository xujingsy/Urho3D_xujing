#pragma once

#include <QHBoxLayout>

class QLabel;
class QSpinBox;
class QSlider;

class IntEditor : public QHBoxLayout
{
    Q_OBJECT

public:
    IntEditor(const QString& text);
    virtual ~IntEditor();

public:
    /// Set value.
    void setValue(int value);
    /// Return value.
    int value() const;

    /// Set range.
    void setRange(int min, int max);
    /// Return range.
    void GetRange(int& min, int& max) const;

signals:
    void valueChanged(int);

protected slots:
    void sliderValueChanged(int value);
    void spinBoxValueChanged(int value);

private:
    QLabel* label_ ;
    QSlider* slider_;
    QSpinBox* spinBox_;
    bool senderIsSlider_;
    bool senderIsSpinBox_;
};
