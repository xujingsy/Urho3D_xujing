#include "stdafx.h"
#include "MainWindow.h"
#include "EditorSceneWidget.h"
#include "Manager/UndoManager.h"
#include "Controls/ButtonsPanel.h"
#include "EditorInfo/EditorGlobalInfo.h"
#include "EditorAPI/EditorEvents.h"
#include "EditorAssist/RTTScene.h"
#include "EditorWindow/AboutDlg.h"

//主要通过Dock分隔窗口
MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags),Object(EditorRoot::Instance()->context_)
{
    showMaximized();

	setWindowIcon(QIcon(":/Images/Icon.png"));

	mModelTool = NULL;

	//
	treeProject = new SceneView(this,EditorRoot::Instance()->context_);
	treeProject->setHeaderLabel("Project");
	treeProject->setMinimumWidth(200);
	treeProject->setMaximumWidth(500);

	//<1>左侧项目栏
	dockLeft = new QDockWidget(this);
	dockLeft->setWindowTitle("Project");
	QSplitter* vLeftSplitter = new QSplitter();
	vLeftSplitter->setOrientation(Qt::Orientation::Vertical);
	vLeftSplitter->addWidget(treeProject);

	//资源
	resTreeView = new ResExplorerView();
	vLeftSplitter->addWidget(resTreeView);

	dockLeft->setWidget(vLeftSplitter);
	addDockWidget(Qt::LeftDockWidgetArea,dockLeft);

	//<2>中间工作区
	pSceneWidget = new EditorSceneWidget();
	setCentralWidget(pSceneWidget);

	//<3>右侧资源，属性
	QSplitter* vResPropSplitter = new QSplitter();
	vResPropSplitter->setOrientation(Qt::Orientation::Vertical);

	dockRight = new QDockWidget(this);
	dockRight->setWindowTitle("Properties");
	dockRight->setMinimumWidth(200);
	dockRight->setMaximumWidth(600);
	dockRight->setFeatures(QDockWidget::AllDockWidgetFeatures);

	dockRight->setWidget(vResPropSplitter);
	addDockWidget(Qt::RightDockWidgetArea,dockRight);

	propertyView = new ObjectPropertiesView();	//属性

	vResPropSplitter->addWidget(propertyView);
	//vResPropSplitter->setMaximumWidth(300);

	//Terrain编辑
	//pTerrainTool = new TerrainEditor();
	//vResPropSplitter->addWidget(pTerrainTool);

	//<4>底部,Console窗口
	logView = new LogView();
	dockBottom = new QDockWidget(this);
	dockBottom->setWindowTitle("Log");
	dockBottom->setWidget(logView);
	addDockWidget(Qt::BottomDockWidgetArea,dockBottom);

	CreateActions();

    CreateMenuBar();
    CreateToolBars();

	statusBar()->showMessage(tr("Ready"));

	EditorRoot::Instance()->SetMainWindow(this);

	OnNewScene();

	//注册一些引擎事件(用于更新UI显示)
	SubscribeToEvent(E_NODE_SELECTION_CHANGE,HANDLER(MainWindow, HandleSelectionChanged));
}

MainWindow::~MainWindow()
{

}

bool MainWindow::eventFilter(QObject *obj,  QEvent *event)
{
	return QMainWindow::eventFilter(obj,event);
}

void MainWindow::HandleSelectionChanged(StringHash eventType, VariantMap& eventData)
{
	int SelCount = EditorRoot::Instance()->SelectionNodes.size();
	renameAction_->setEnabled(SelCount == 1);
	copyAction_->setEnabled(SelCount > 0);
	cutAction_->setEnabled(SelCount > 0);

	deleteAction_->setEnabled(SelCount > 0);
	btnAttachTerrain->setEnabled(SelCount > 0);
}

void MainWindow::CreateActions()
{
	//打开场景文件
	openAction_ = new QAction(QIcon(":/Images/Actions/Open.png"), tr("Open ..."), this);
	openAction_->setShortcut(QKeySequence::fromString("Ctrl+0"));
	connect(openAction_, SIGNAL(triggered(bool)), this, SLOT(HandleOpenAction()));

	saveAction_ = new QAction(QIcon(":/Images/Actions/Save.png"), tr("Save ..."), this);
	saveAction_->setShortcut(QKeySequence::fromString("Ctrl+S"));
	connect(saveAction_, SIGNAL(triggered(bool)), this, SLOT(HandleSaveAction()));

	exitAction_ = new QAction(QIcon(":/Images/Actions/Exit.png"), tr("Exit ..."), this);

	undoAction_ = new QAction(QIcon(":/Images/Actions/Undo.png"), tr("Undo"), this);
	connect(undoAction_,SIGNAL(triggered()), this, SLOT(HandleUndoAction()));
	undoAction_->setShortcut(QKeySequence::fromString("Ctrl+Z"));

	redoAction_ = new QAction(QIcon(":/Images/Actions/Redo.png"), tr("Redo"), this);
	connect(redoAction_,SIGNAL(triggered()), this, SLOT(HandleRedoAction()));
	redoAction_->setShortcut(QKeySequence::fromString("Ctrl+Y"));

	screenshot_ = new QAction(QIcon(":/Images/Actions/Camera.png"), tr("Screenshot"), this);
	connect(screenshot_,SIGNAL(triggered()),this,SLOT(HandleScreenshotAction()));

	renameAction_ = new QAction(QIcon(":/Images/Actions/Rename.png"),tr("Rename"), this);

	//剪切,复制,粘贴
	copyAction_ = new QAction(QIcon(":/Images/Actions/Copy.png"),tr("Copy"), this);
	copyAction_->setShortcut(QKeySequence::fromString("Ctrl+C"));
	connect(copyAction_,SIGNAL(triggered()), this, SLOT(HandleCopyAction()));

	cutAction_ = new QAction(QIcon(":/Images/Actions/Cut.png"),tr("Cut"), this);
	cutAction_->setShortcut(QKeySequence::fromString("Ctrl+X"));
	connect(cutAction_,SIGNAL(triggered()),this,SLOT(HandleCutAction()));

	pasteAction_ = new QAction(QIcon(":/Images/Actions/Paste.png"),tr("Paste"), this);
	pasteAction_->setShortcut(QKeySequence::fromString("Ctrl+V"));
	connect(pasteAction_,SIGNAL(triggered()), this, SLOT(HandlePasteAction()));

	deleteAction_ = new QAction(QIcon(":/Images/Actions/Delete.png"),tr("Delete"), this);
	deleteAction_->setShortcut(QKeySequence::Delete);
	connect(deleteAction_,SIGNAL(triggered()),this,SLOT(HandleDeleteAction()));

	//工具菜单栏
	modelTransAction_ = new QAction(QIcon(":/Images/Actions/ModelTool.png"), tr("Model Tool"), this);
	connect(modelTransAction_,SIGNAL(triggered()), this, SLOT(HandleModelToolAction()));

	helpAction_ = new QAction(QIcon(":/Images/Actions/Help.png"), tr("Help"), this);
	aboutAction_ = new QAction(QIcon(":/Images/Actions/Info.png"), tr("About"), this);
	connect(aboutAction_, SIGNAL(triggered()), this, SLOT(HandleAbout()));

	//Select
	selectAction_ = new QAction(QIcon(":/Images/Actions/Select.png"), tr("Select"), this);
	selectAction_->setStatusTip(tr("Select"));
	selectAction_->setEnabled(true);
	selectAction_->setCheckable(true);
	selectAction_->setChecked(true);
	connect(selectAction_,SIGNAL(triggered(bool)), this,SLOT(HandleSelectTool(bool)));

	//Move
	moveAction_ = new QAction(QIcon(":/Images/Actions/Move.png"), tr("Move"), this);
	moveAction_->setStatusTip(tr("Move"));
	moveAction_->setEnabled(true);
	moveAction_->setCheckable(true);
	moveAction_->setChecked(false);
	connect(moveAction_,SIGNAL(triggered(bool)), this, SLOT(HandleMoveTool(bool)));

	//Rotate
	rotateAction_ = new QAction(QIcon(":/Images/Actions/Rotate.png"), tr("Rotate"),this);
	rotateAction_->setStatusTip(tr("Rotate"));
	rotateAction_->setEnabled(true);
	rotateAction_->setCheckable(true);
	rotateAction_->setChecked(false);
	connect(rotateAction_,SIGNAL(triggered(bool)), this, SLOT(HandleRotateTool(bool)));
}

void MainWindow::CreateMenuBar()
{
	//文件菜单
    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(openAction_);
	fileMenu->addAction(saveAction_);
	fileMenu->addAction(exitAction_);

	//编辑菜单
    QMenu* editMenu = menuBar()->addMenu(tr("&Edit"));
	editMenu->addAction(undoAction_);
	editMenu->addAction(redoAction_);
	editMenu->addSeparator();

	editMenu->addAction(screenshot_);
	editMenu->addAction(renameAction_);
	editMenu->addSeparator();

	editMenu->addAction(copyAction_);
	editMenu->addAction(cutAction_);
	editMenu->addAction(pasteAction_);
	editMenu->addAction(deleteAction_);

	//工具菜单
	QMenu* toolsMenu = menuBar()->addMenu(tr("Tools"));
	toolsMenu->addAction(modelTransAction_);

	QMenu* helpMenu = menuBar()->addMenu(tr("Help"));
	helpMenu->addAction(helpAction_);
	helpMenu->addAction(aboutAction_);
}

void MainWindow::OnNewScene()
{
	renameAction_->setEnabled(false);
	copyAction_->setEnabled(false);
	cutAction_->setEnabled(false);
	deleteAction_->setEnabled(false);

	updateUndoRedoActions();
}

//工具条上的按钮
void MainWindow::CreateToolBars()
{
	fileToolBar = addToolBar(tr("File"));
	fileToolBar->setIconSize(QSize(32,32));

	fileToolBar->addAction(openAction_);
	fileToolBar->addAction(saveAction_);
	fileToolBar->addSeparator();
	fileToolBar->addAction(undoAction_);
	fileToolBar->addAction(redoAction_);
	fileToolBar->addSeparator();
	
	//
	addToolBarBreak();
	
	editToolBar = addToolBar(tr("Edit"));
	editToolBar->setIconSize(QSize(32,32));

	QComboBox* cboFillMode = new QComboBox();
	cboFillMode->addItem("Solid");
	cboFillMode->addItem("WireFrame");
	cboFillMode->addItem("Point");
	fileToolBar->addWidget(cboFillMode);
	connect(cboFillMode,SIGNAL(currentIndexChanged(int)),this,SLOT(onModeChanged(int)));

	fileToolBar->addSeparator();
	fileToolBar->addAction(screenshot_);
	fileToolBar->addSeparator();

	editToolBar->addAction(selectAction_);
	editToolBar->addAction(moveAction_);
	editToolBar->addAction(rotateAction_);

	editToolBar->addSeparator();
	editToolBar->addAction(renameAction_);
	editToolBar->addAction(cutAction_);
	editToolBar->addAction(copyAction_);
	editToolBar->addAction(pasteAction_);
	editToolBar->addSeparator();

	btnAttachTerrain = new QPushButton(QIcon(":/Images/Actions/Attach.png"),QString::fromLocal8Bit("吸附到地表"));
	editToolBar->addWidget(btnAttachTerrain);
	
	QPushButton* pTestEffect = new QPushButton(QIcon(":/Images/Components/ParticleEmitter.png"),QString::fromLocal8Bit("添加特效"));
	editToolBar->addWidget(pTestEffect);

	connect(btnAttachTerrain,SIGNAL(clicked(bool)),this,SLOT(HandleAttachTerrainAction(bool)));
	connect(pTestEffect,SIGNAL(clicked(bool)),this,SLOT(HandleTestEffectAction(bool)));
}

void MainWindow::HandleAttachTerrainAction(bool)
{
	EditorRoot::Instance()->AttachSelectionsToTerrain();
}

void MainWindow::HandleTestEffectAction(bool)
{
	ResourceCache* cache = GetSubsystem<ResourceCache>();

	//EditorRoot::Instance()->AddEffetToSelectionNodes();
	RTTScene* rttScene = EditorRoot::Instance()->GetRTTScene();

	Node* objNode = rttScene->CreateChildNode("testObj");
	StaticModel* pModel = objNode->CreateComponent<StaticModel>();
	pModel->SetModel(cache->GetResource<Model>("Models/Plane.mdl"));
	pModel->SetMaterial(cache->GetResource<Material>("Materials/StoneTiled.xml"));

	rttScene->SaveJpg("D:/aa.png");
}

void MainWindow::HandleModelToolAction()
{
	if(mModelTool == NULL)
		mModelTool = new ModelTool();

	mModelTool->show();
}

void MainWindow::setApplicationObject(QObject* obj)
{
	obj->installEventFilter(this);
}

void MainWindow::HandleOpenAction()
{
	QString fileName = QFileDialog::getOpenFileName(0, tr("Open Scene Xml File"), "./Data/Scenes/", "*.xml");
	if (fileName.isEmpty())
		return;

	EditorRoot::Instance()->OpenScene(fileName.toLatin1().data());
}

void MainWindow::HandleSaveAction()
{
	const String fileName = EditorRoot::Instance()->GetFileName();
	if(fileName == "")
	{
		QString  selFileName = QFileDialog::getSaveFileName(this, tr("Save Scene"), "./Data/Scene/", "*.xml");
		if(selFileName.isEmpty())
			return;

		EditorRoot::Instance()->SaveScene(selFileName.toStdString().c_str());
	}
	else
	{
		EditorRoot::Instance()->SaveScene(fileName.CString());
	}
}

void MainWindow::HandleSelectTool(bool checked)
{
	EditorRoot::Instance()->mSelectedTool = TOOL_SELECT;

	updateActions();

	EditorRoot::Instance()->GetGizmo()->SetMode(enEditMode_Move);
}

void MainWindow::HandleMoveTool(bool checked)
{
	EditorRoot::Instance()->mSelectedTool = TOOL_MOVE;

	updateActions();

	EditorRoot::Instance()->GetGizmo()->SetMode(enEditMode_Move);
}

void MainWindow::HandleRotateTool(bool checked)
{
	EditorRoot::Instance()->mSelectedTool = TOOL_ROTATE;

	updateActions();

	EditorRoot::Instance()->GetGizmo()->SetMode(enEditMode_Rotate);
}

void MainWindow::HandleCutAction()
{
	EditorRoot::Instance()->GetObjectPositionEditor()->OnNodesCut();
}

void MainWindow::HandleCopyAction()
{
	EditorRoot::Instance()->GetObjectPositionEditor()->OnNodesCopy();
}

void MainWindow::HandlePasteAction()
{
	EditorRoot::Instance()->GetObjectPositionEditor()->OnNodesPaste();
}

void MainWindow::HandleDeleteAction()
{
	int result = QMessageBox::question(this,"Question","Realy Delete?",QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Ok);
	switch(result)
	{
	case QMessageBox::Ok:
		EditorRoot::Instance()->DeleteAllSelectionNodes();
		break;
	default:
		break;
	}
}

void MainWindow::HandleUndoAction()
{
	UndoManager::Instance()->Undo();
	updateUndoRedoActions();
}

void MainWindow::HandleRedoAction()
{
	UndoManager::Instance()->Redo();
	updateUndoRedoActions();
}

void MainWindow::updateUndoRedoActions()
{
	undoAction_->setToolTip(UndoManager::Instance()->GetUndoString().c_str());
	undoAction_->setEnabled(UndoManager::Instance()->CanUndo());

	redoAction_->setToolTip(UndoManager::Instance()->GetRedoString().c_str());
	redoAction_->setEnabled(UndoManager::Instance()->CanRedo());
}

void MainWindow::updateActions()
{
	selectAction_->setChecked(EditorRoot::Instance()->mSelectedTool == TOOL_SELECT);
	moveAction_->setChecked(EditorRoot::Instance()->mSelectedTool == TOOL_MOVE);
	rotateAction_->setChecked(EditorRoot::Instance()->mSelectedTool == TOOL_ROTATE);

	EditorRoot::Instance()->ActiveTool = EditorRoot::Instance()->mSelectedTool;
}

void MainWindow::HandleScreenshotAction()
{
	Graphics* graphics = gEditorGlobalInfo->GetSubsystem<Graphics>();
	Image screenshot(gEditorGlobalInfo->GetContext());
	graphics->TakeScreenShot(screenshot);

	String fileName = gEditorGlobalInfo->GetSubsystem<FileSystem>()->GetProgramDir() + "Data/Screenshot_" +
		Time::GetTimeStamp().Replaced(':', '_').Replaced('.', '_').Replaced(' ', '_') + ".png";

	// Here we save in the Data folder with date and time appended
	screenshot.SavePNG(fileName);

	char szMsg[256];
	sprintf(szMsg,"Saved:\r\n%s",fileName.CString());

	QMessageBox msg(QMessageBox::Information,"Saved",szMsg);
	msg.setIconPixmap(QPixmap(":/Images/Actions/Camera.png"));
	msg.exec();
}

void MainWindow::AddLog(int type,const char* logContent)
{
	logView->AddLog(type,logContent);
}

void MainWindow::onModeChanged(int index)
{
	switch(index)
	{
	case FILL_SOLID:
	case FILL_WIREFRAME:
	case FILL_POINT:
		gEditorGlobalInfo->SetFillMode((FillMode)index);
		break;
	}
}

void MainWindow::HandleAbout()
{
	AboutDlg dlg;
	dlg.exec();
}
