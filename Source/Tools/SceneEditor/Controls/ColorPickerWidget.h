#pragma once
#include <qwidget.h>
#include <qcolor.h>
#include <qpainter.h>
#include "Color.h"	//Urho Color

class ColorPickerWidget : public QWidget
{
Q_OBJECT
public:
	ColorPickerWidget(QWidget* parent,QColor color) : QWidget(parent)
	{
		mColor = color;
	}

	virtual ~ColorPickerWidget()
	{
	}

	Urho3D::Color getColor();
	void setColor(Urho3D::Color color);
Q_SIGNALS:
	void colorChanged(Urho3D::Color color);
protected:
	QColor mColor;
	void paintEvent(QPaintEvent* evt);
	void mousePressEvent(QMouseEvent* evt);
};