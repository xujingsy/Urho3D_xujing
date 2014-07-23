#include "stdafx.h"
#include "MainWindow.h"
#include "QColorDialog"
#include "QFileDialog"
#include "EditorApplication.h"

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
	connect(newAction_, SIGNAL(triggered(bool)), this, SLOT(HandleNewAction()));

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

	backgroundAction_ = new QAction(tr("Background"), this);
	backgroundAction_->setShortcut(QKeySequence::fromString("Ctrl+B"));
	connect(backgroundAction_, SIGNAL(triggered(bool)), this, SLOT(HandleBackgroundAction()));
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
	viewMenu_->addAction(backgroundAction_);
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
	EditorApplication::Get()->New();
}

void MainWindow::HandleOpenAction()
{
	QString fileName = QFileDialog::getOpenFileName(0, tr("Open 3D Particle"), "./Data/Particle/", "*.xml");
	if(fileName.isEmpty())
		return;

	EditorApplication::Get()->Open(fileName.toLatin1().data());
}

void MainWindow::HandleSaveAction()
{
	const String& fileName = EditorApplication::Get()->GetFileName();
	if(fileName.Empty())
	{
		QString fileName = QFileDialog::getSaveFileName(0, tr("Open 3D Particle"), "./Data/Particle/", "*.xml");
		if (fileName.isEmpty())
			return;

		EditorApplication::Get()->Save(fileName.toLatin1().data());
	}
	else
	{
		EditorApplication::Get()->Save(fileName);
	}
}

void MainWindow::HandleZoomAction()
{
	Camera* camera = EditorApplication::Get()->GetCamera();

	QObject* s = sender();
	float fZoom = camera->GetZoom();

	if(s == zoomInAction_)
	{
		fZoom *= 1.25f;
		if(fZoom > 10.f)
			fZoom = 10.f;
		camera->SetZoom(fZoom);
	}
	else if(s == zoomOutAction_)
	{
		fZoom *= 0.8f;
		if(fZoom < 0.1f)
			fZoom = 0.1f;
		camera->SetZoom(fZoom);
	}
	else if(s == zoomResetAction_)
	{
		camera->SetZoom(1.0f);
	}
}

void MainWindow::HandleBackgroundAction()
{
	Renderer* renderer = GetSubsystem<Renderer>();

	const Color& color = renderer->GetDefaultZone()->GetFogColor();

	QColor qColor = QColor::fromRgbF(color.r_, color.g_, color.b_);
	QColor newQcolor = QColorDialog::getColor(qColor, this);

	Color newColor(newQcolor.redF(), newQcolor.greenF(), newQcolor.blueF());
	renderer->GetDefaultZone()->SetFogColor(newColor);
}
