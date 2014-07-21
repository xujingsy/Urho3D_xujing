#include "stdafx.h"
#include "LogView.h"

LogView::LogView(QWidget* parent)
{
	debugCount_ = 0;
	infoCount_ = 0;
	warningCount_ = 0;
	errorCount_ = 0;

	QBoxLayout* layout = new QBoxLayout(QBoxLayout::TopToBottom);
	this->setLayout(layout);

	toolBar_ = new QToolBar();
	toolBar_->setMovable(false);
	layout->addWidget(toolBar_);

	debugAct_ = new QPushButton(QIcon(":/Images/Log/Debug.png"), "", this);
	debugAct_->setFlat(true);
	debugAct_->setCheckable(true);
	toolBar_->addWidget(debugAct_);
	toolBar_->addSeparator();

	infoAct_ = new QPushButton(QIcon(":/Images/Log/Info.png"), "", this);
	infoAct_->setCheckable(true);
	toolBar_->addWidget(infoAct_);
	toolBar_->addSeparator();

	warningAct = new QPushButton(QIcon(":/Images/Log/Warning.png"), "", this);
	warningAct->setCheckable(true);
	warningAct->setChecked(true);
	toolBar_->addWidget(warningAct);
	toolBar_->addSeparator();
	
	errorAct = new QPushButton(QIcon(":/Images/Log/Error.png"), "", this);
	errorAct->setCheckable(true);
	errorAct->setChecked(true);
	toolBar_->addWidget(errorAct);

	update_total_info(LOG_DEBUG);
	update_total_info(LOG_INFO);
	update_total_info(LOG_WARNING);
	update_total_info(LOG_ERROR);

	//log窗口
	logWidget_ = new QListWidget();
	logWidget_->setSortingEnabled(true);
	layout->addWidget(logWidget_);

	connect(debugAct_,SIGNAL(clicked(bool)),this,SLOT(on_filter_click(bool)));
	connect(infoAct_,SIGNAL(clicked(bool)),this,SLOT(on_filter_click(bool)));
	connect(warningAct,SIGNAL(clicked(bool)),this,SLOT(on_filter_click(bool)));
	connect(errorAct,SIGNAL(clicked(bool)),this,SLOT(on_filter_click(bool)));
}

void LogView::on_filter_click(bool checked)
{
	update_log_items();
}

void LogView::update_log_items()
{
	int rowCount = logWidget_->count();
	for(int i = 0;i < rowCount;i ++)
	{
		QListWidgetItem* pRow = logWidget_->item(i);
		update_single_item(pRow);
	}
}

void LogView::update_single_item(QListWidgetItem* pRow)
{
	int type = pRow->whatsThis().toInt();
	switch(type)
	{
	case LOG_DEBUG:
		pRow->setHidden(!debugAct_->isChecked());
		break;
	case LOG_INFO:
		pRow->setHidden(!infoAct_->isChecked());
		break;
	case LOG_WARNING:
		pRow->setHidden(!warningAct->isChecked());
		break;
	case LOG_ERROR:
		pRow->setHidden(!errorAct->isChecked());
		break;
	}
}

void LogView::AddLog(int type,const char* logContent)
{
	QListWidgetItem *item = new QListWidgetItem();
	item->setWhatsThis(QString::number(type));	//记录日志类型，为了显示过滤时使用

	QColor fontColor;
	switch(type)
	{
	case LOG_DEBUG:
		debugCount_ ++;
		fontColor = QColor("#C0C0C0");
		item->setIcon(QIcon("Images/debug.png"));
		break;
	case LOG_INFO:
		infoCount_ ++;
		fontColor = QColor("#C0C0C0");
		item->setIcon(QIcon("Images/info.png"));
		break;
	case LOG_WARNING:
		warningCount_ ++;
		fontColor = QColor("#FFCC00");
		item->setIcon(QIcon("Images/warning.png"));
		break;
	case LOG_ERROR:
		errorCount_ ++;
		fontColor = QColor("#EE1100");
		item->setIcon(QIcon("Images/error.png"));
		break;
	}

	update_total_info(type);

	item->setTextColor(fontColor);
	item->setText(logContent);

	logWidget_->insertItem(0,item);

	update_single_item(item);
}

void LogView::update_total_info(int type)
{
	switch(type)
	{
	case LOG_DEBUG:
		debugAct_->setText(QString::number(debugCount_) + " Debugs");
		break;
	case LOG_INFO:
		infoAct_->setText(QString::number(infoCount_) + " Infos");
		break;
	case LOG_WARNING:
		warningAct->setText(QString::number(warningCount_) + " Warnings");
		break;
	case LOG_ERROR:
		errorAct->setText(QString::number(errorCount_) + " Errors");
		break;
	}
}
