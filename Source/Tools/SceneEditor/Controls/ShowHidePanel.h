#pragma once
#include <qwidget.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qboxlayout.h>
#include <QGroupBox>

namespace Urho3D
{
	class Node;
}

using namespace Urho3D;

class ShowHidePanel : public QWidget
{
	Q_OBJECT
public:
	ShowHidePanel(QWidget* parent = NULL);
	void SetIcon(const char* iconSrc);
	void SetName(const char* name);
	void Show();
	void Hide();
private slots:
	void buttonClicked(bool checked);
protected:
	QVBoxLayout* vLayout;
	QGroupBox* frame_;
	QVBoxLayout* frameLayout_;
	QHBoxLayout* getNewRow();
private:
	QPushButton* lblButton_;
	QLabel* lblIcon_;
	QLabel* lblName_;
};
