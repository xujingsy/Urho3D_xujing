#pragma once
#include <QWidget>
#include <QLabel>
#include <QLineEdit>

#include <Windows.h>	//todo:使用Qt自身的坐标

class LabelTextBox : public QWidget
{
	Q_OBJECT
public:
	LabelTextBox(QWidget* parent = 0);
	~LabelTextBox();

	void SetName(const char* pName);
	void SetText(const char* pText);
	void SetReadonly(bool isReadonly);

	QString GetText()
	{
		return txtValue->text();
	}

	QLabel* lblName;
	QLineEdit* txtValue;

	void SetValueRange(float from,float to);
signals:
	void valueChanged(float);
private slots:
	void textChanged(const QString & text);
private:
	bool eventFilter(QObject* target,QEvent* event);

	bool isReadOnly;
	bool isNumberType_;
	bool m_Captured;
	POINT ptDown;
	float initValue;
	float valueFrom;
	float valueTo;
};
