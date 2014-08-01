#pragma once
#include <QWidget>
#include <QPushButton>
#include "Str.h"
using namespace Urho3D;

class ButtonArrayPanel : public QWidget
{
	Q_OBJECT
public:
	ButtonArrayPanel(QWidget* parent = 0);
	virtual ~ButtonArrayPanel();

	QPushButton* AddButton(String iconName,String text);

};
