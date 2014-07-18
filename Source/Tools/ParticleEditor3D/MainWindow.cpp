#include "stdafx.h"
#include "MainWindow.h"

MainWindow::MainWindow(Context* context) :
	QMainWindow(0, 0), Object(context)
{
	setWindowIcon(QIcon(":/Images/Icon.png"));
	showMaximized();

	//Center Main Widget
	QWidget* widget = new QWidget();
	setCentralWidget(widget);

	widget->setMinimumSize(128, 128);
	widget->setUpdatesEnabled(false);
	widget->setFocusPolicy(Qt::StrongFocus);
}

