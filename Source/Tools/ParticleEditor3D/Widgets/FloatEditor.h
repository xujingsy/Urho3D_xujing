#pragma once

#include <QHBoxLayout>

class QLabel;
class QDoubleSpinBox;
class QSlider;

class FloatEditor : public QHBoxLayout
{
    Q_OBJECT

public:
    FloatEditor(const QString& text, bool setLabelWidth = true);
    virtual ~FloatEditor();

public:
    /// Set value.
    void setValue(float value);
    /// Return value.
    float value() const;

    /// Set range.
    void setRange(float min, float max);
    
    QLabel* label() const { return label_; }
    QSlider* slider() const { return slider_; }
    QDoubleSpinBox* spinBox() const { return spinBox_; }

signals:
    void valueChanged(float);

protected slots:
    void sliderValueChanged(int value);
    void spinBoxValueChanged(double value);

private:
    void setSliderValue(double value);

    QLabel* label_ ;
    QSlider* slider_;
    QDoubleSpinBox* spinBox_;
    bool senderIsSlider_;
    bool senderIsSpinBox_;
};
