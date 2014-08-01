#include "stdafx.h"
#include "AboutDlg.h"

AboutDlg::AboutDlg(QWidget* parent)
{
	setWindowTitle(tr("About"));
	resize(425, 266);

	QLabel* lblIcon = new QLabel("", this);

	QPixmap bmp(QString::fromUtf8(":/Images/Icon.png"));
	lblIcon->setPixmap(bmp);
	lblIcon->setGeometry(QRect(225, 75, 136, 137));

	QLabel* lblInfo = new QLabel("Urho3D Scene Editor", this);
	lblInfo->setGeometry(QRect(50, 30, 250, 36));
	lblInfo->setFont(QFont("Times", 20, QFont::Bold));

	QLabel* lblVersion = new QLabel("Version: 0.0.0.1", this);
	lblVersion->setGeometry(QRect(50, 130, 125, 30));
	lblVersion->setFont(QFont("Times", 14));

	QPushButton* btnOK = new QPushButton("OK", this);
	btnOK->setFixedWidth(120);

	connect(btnOK, SIGNAL(clicked()), this, SLOT(accept()));
	btnOK->setGeometry(QRect(175, 230, 75, 23));
}

AboutDlg::~AboutDlg()
{

}
