#pragma once
#include <QDialog>

class AboutDlg : public QDialog
{
Q_OBJECT
public:
	AboutDlg(QWidget* parent = 0);
	virtual ~AboutDlg();

private slots:
	virtual void accept();
private:

};
