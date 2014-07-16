#include "stdafx.h"
#include "ColorPickerWidget.h"
#include <qcolordialog.h>

Urho3D::Color ColorPickerWidget::getColor()
{
	return Color(mColor.red(),mColor.green(),mColor.blue());
}

void ColorPickerWidget::setColor(Urho3D::Color color)
{
	mColor.setRed(color.r_);
	mColor.setGreen(color.g_);
	mColor.setBlue(color.b_);

	update();
}

void ColorPickerWidget::paintEvent(QPaintEvent* evt)
{
	QPainter painter(this);
	painter.setClipRect(2,2,width() - 2,height() - 2);
	painter.setBrush(QBrush(mColor));
	painter.setPen(QColor(0,0,0));
	painter.fillRect(QRectF(0,0,width(),height()),mColor);
	painter.drawRect(2,2,width() - 3,height() - 3);
}

void ColorPickerWidget::mousePressEvent(QMouseEvent* evt)
{
	if(evt->button() == Qt::LeftButton)
	{
		QColor result = QColorDialog::getColor(mColor,this);
		if(result.isValid())
		{
			mColor = result;
			update();

			Urho3D::Color value = getColor();
			emit colorChanged(value);
		}
	}
}
