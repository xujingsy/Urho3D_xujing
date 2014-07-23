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

void MainWindow::CreateWidgets()
{
	CreateActions();
	CreateMenuBar();
	CreateToolBar();
	CreateDockWidgets();
}

void MainWindow::CreateActions()
{
	newAction_ = new QAction(QIcon(":/Images/New.png"), tr("New"), this);
	newAction_->setShortcut(QKeySequence::fromString("Ctrl+N"));
	connect(newAction_, SIGNAL(triggered(bool)), this, SLOT(HandleNewAction));

	openAction_ = new QAction(QIcon(":/Images/Open.png"), tr("Open..."), this);
	openAction_->setShortcut(QKeySequence::fromString("Ctrl+O"));
	connect(openAction_, SIGNAL(triggered(bool)), this, SLOT(HandleOpenAction()));

	saveAction_ = new QAction(QIcon(":/Images/Save.png"), tr("Save"), this);
	saveAction_->setShortcut(QKeySequence::fromString("Ctrl+S"));
	connect(saveAction_, SIGNAL(triggered(bool)), this, SLOT(HandleSaveAction()));

	zoomInAction_ = new QAction(QIcon(":/Images/ZoomIn.png"), tr("Zoom In"), this);
	zoomInAction_->setShortcut(QKeySequence::fromString("Ctrl++"));
	connect(zoomInAction_, SIGNAL(triggered(bool)), this, SLOT(HandleZoomAction()));

	zoomOutAction_ = new QAction(QIcon(":/Images/ZoomOut.png"), tr("Zoom Out"), this);
	zoomOutAction_->setShortcut(QKeySequence::fromString("Ctrl+-"));
	connect(zoomOutAction_, SIGNAL(triggered(bool)), this, SLOT(HandleZoomAction()));

	zoomResetAction_ = new QAction(QIcon(":/Images/ZoomReset.png"), tr("Zoom Reset"), this);
	zoomResetAction_->setShortcut(QKeySequence::fromString("Ctrl+0"));
	connect(zoomResetAction_, SIGNAL(triggered(bool)), this, SLOT(HandleZoomAction()));

	exitAction_ = new QAction(tr("Exit"), this);
	exitAction_->setShortcut(QKeySequence::fromString("Alt+F4"));
	connect(exitAction_, SIGNAL(triggered(bool)), this, SLOT(close()));
}

void MainWindow::CreateMenuBar()
{
	fileMenu_ = menuBar()->addMenu(tr("&File"));

	fileMenu_->addAction(newAction_);
	fileMenu_->addAction(openAction_);
	fileMenu_->addAction(saveAction_);
	fileMenu_->addSeparator();

	viewMenu_ = menuBar()->addMenu(tr("&View"));
	viewMenu_->addAction(zoomInAction_);
	viewMenu_->addAction(zoomOutAction_);
	viewMenu_->addAction(zoomResetAction_);
}

void MainWindow::CreateToolBar()
{
	toolBar_ = addToolBar(tr("ToolBar"));

	toolBar_->addAction(newAction_);
	toolBar_->addAction(openAction_);
	toolBar_->addAction(saveAction_);

	toolBar_->addSeparator();

	toolBar_->addAction(zoomInAction_);
	toolBar_->addAction(zoomOutAction_);
	toolBar_->addAction(zoomResetAction_);
}

void MainWindow::CreateDockWidgets()
{

}

void MainWindow::HandleNewAction()
{

}

void MainWindow::HandleOpenAction()
{

}

void MainWindow::HandleSaveAction()
{

}

void MainWindow::HandleZoomAction()
{

}
