#pragma once
#include <qwidget.h>
#include <qlistwidget.h>
#include <qtoolbar.h>
#include <qpushbutton.h>

class LogView : public QWidget
{
	Q_OBJECT
public:
	LogView(QWidget* parent = 0);

	void AddLog(int type,const char* logContent);
private slots:
	void on_filter_click(bool checked = false);
private:
	QToolBar* toolBar_;
	QListWidget* logWidget_;

	QPushButton* debugAct_;
	QPushButton* infoAct_;
	QPushButton* warningAct;
	QPushButton* errorAct;

	int debugCount_;
	int infoCount_;
	int warningCount_;
	int errorCount_;

	void update_total_info(int type);
	void update_log_items();
	void update_single_item(QListWidgetItem* pRow);
};