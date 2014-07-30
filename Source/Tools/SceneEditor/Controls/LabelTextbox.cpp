#include "stdafx.h"
#include "LabelTextbox.h"

LabelTextBox::LabelTextBox(QWidget* parent)
{
	m_Captured = false;
	initValue = 0;
	isNumberType_ = false;
	valueFrom = 0;
	valueTo = 100;

	isReadOnly = false;

	QHBoxLayout* layout = new QHBoxLayout();
	layout->setSpacing(1);
	this->setLayout(layout);

	lblName = new QLabel();
	lblName->setText("Name");
	layout->addWidget(lblName);

	txtValue = new QLineEdit();
	txtValue->setFixedWidth(50);
	layout->addWidget(txtValue);

	//注册事件
	lblName->installEventFilter(this);
	txtValue->installEventFilter(this);

	connect(txtValue,SIGNAL(textChanged(const QString &)),this,SLOT(textChanged(const QString &)));
}

LabelTextBox::~LabelTextBox()
{

}

void LabelTextBox::textChanged(const QString & text)
{
	emit valueChanged(text.toFloat());
}

void LabelTextBox::SetValueRange(float from,float to)
{
	//只有数字类型才显示
	lblName->setCursor(Qt::SplitHCursor);
	isNumberType_ = true;

	valueFrom = from;
	valueTo = to;
}

void LabelTextBox::SetName(const char* pName)
{
	lblName->setText(pName);
}

void LabelTextBox::SetText(const char* pText)
{
	txtValue->setText(pText);
}

void LabelTextBox::SetReadonly(bool isReadonly)
{
	txtValue->setReadOnly(isReadonly);
}

bool LabelTextBox::eventFilter(QObject* target,QEvent* event)
{
	if(isReadOnly == false && isNumberType_ == true && target == lblName)
	{
		int eType = event->type();
		if(eType == QEvent::MouseMove || eType == QEvent::MouseButtonPress || eType == QEvent::MouseButtonRelease)
		{
			QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);

			if(eType == QEvent::MouseMove)
			{
				int dX = mouseEvent->globalX() - ptDown.x;
				float fValue = initValue + dX * 0.1;
				txtValue->setText(QString::number(fValue));
			}
			else if(eType == QEvent::MouseButtonPress)
			{
				if(mouseEvent->button() == Qt::LeftButton)
				{
					GetCursorPos(&ptDown);
					initValue = txtValue->text().toFloat();
					SetCapture(lblName->winId());
				}
			}
			else if(eType == QEvent::MouseButtonRelease)
			{
				if(m_Captured == true)
				{
					ReleaseCapture();
				}
			}
		}
	}

	return QWidget::eventFilter(target,event);
}
