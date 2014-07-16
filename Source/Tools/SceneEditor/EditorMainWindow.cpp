#include "stdafx.h"
#include "EditorMainWindow.h"
#include "EditorSceneWidget.h"
#include "Manager/UndoManager.h"
#include "Controls/ButtonsPanel.h"
#include "EditorInfo/EditorGlobalInfo.h"
#include "EditorAPI/EditorEvents.h"
#include <qmessagebox.h>
#include "EditorAssist/RTTScene.h"

//主要通过Dock分隔窗口
EditorMainWindow::EditorMainWindow(QWidget *parent, Qt::WindowFlags flags) : QMainWindow(parent, flags),Object(EditorsRoot::Instance()->context_)
{
    showMaximized();

	mModelTool = NULL;

	//
	treeProject = new SceneView(this,EditorsRoot::Instance()->context_);
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

    CreateMenuBar();
    CreateToolBars();

	statusBar()->showMessage(tr("Ready"));

	EditorsRoot::Instance()->SetMainWindow(this);

	OnNewScene();

	//注册一些引擎事件(用于更新UI显示)
	SubscribeToEvent(E_NODE_SELECTION_CHANGE,HANDLER(EditorMainWindow,HandleSelectionChanged));
}

EditorMainWindow::~EditorMainWindow()
{

}

bool EditorMainWindow::eventFilter(QObject *obj,  QEvent *event)
{
	return QMainWindow::eventFilter(obj,event);
}

void EditorMainWindow::HandleSelectionChanged(StringHash eventType, VariantMap& eventData)
{
	int SelCount = EditorsRoot::Instance()->SelectionNodes.size();
	renameAction_->setEnabled(SelCount == 1);
	copyAction_->setEnabled(SelCount > 0);
	cutAction_->setEnabled(SelCount > 0);

	deleteAction_->setEnabled(SelCount > 0);
	btnAttachTerrain->setEnabled(SelCount > 0);
}

void EditorMainWindow::CreateMenuBar()
{
    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
    openAction_ = fileMenu->addAction(QIcon("Images/fileopen.png"),tr("Open ..."));
    saveAction_ = fileMenu->addAction(QIcon("Images/filesave.png"),tr("Save ..."));
	exitAction_ = fileMenu->addAction(QIcon("Images/exit.png"),tr("Exit ..."));

    QMenu* editMenu = menuBar()->addMenu(tr("&Edit"));
	undoAction_ = editMenu->addAction(QIcon("Images/editundo.png"),tr("Undo"));
	redoAction_ = editMenu->addAction(QIcon("Images/editredo.png"),tr("Redo"));
	editMenu->addSeparator();

	screenshot_ = editMenu->addAction(QIcon("Images/camera.png"),tr("Screenshot"));

	renameAction_ = editMenu->addAction(QIcon("Images/Rename.png"),tr("Rename"));
	
	editMenu->addSeparator();

	copyAction_ = editMenu->addAction(QIcon("Images/Copy.png"),tr("Copy"));
	
	cutAction_ = editMenu->addAction(QIcon("Images/Cut.png"),tr("Cut"));
	pasteAction_ = editMenu->addAction(QIcon("Images/Paste.png"),tr("Paste"));
	deleteAction_ = editMenu->addAction(QIcon("Images/Delete.png"),tr("Delete"));

	QMenu* toolsMenu = menuBar()->addMenu(tr("Tools"));
	QAction* modelTransAction_ = toolsMenu->addAction(QIcon("Images/ModelTool.png"),tr("Model Tool"));
	connect(modelTransAction_,SIGNAL(triggered()), this, SLOT(cmdModelTool()));

	QMenu* helpMenu = menuBar()->addMenu(tr("Help"));
	QAction* helpAction_ = helpMenu->addAction(QIcon("Images/help.png"),tr("Help"));
	QAction* aboutAction_ = helpMenu->addAction(QIcon("Images/info.png"),tr("About"));

	connect(undoAction_,SIGNAL(triggered()), this, SLOT(cmdUndo()));
	connect(redoAction_,SIGNAL(triggered()), this, SLOT(cmdRedo()));
	connect(screenshot_,SIGNAL(triggered()),this,SLOT(onScreenshot()));

	//剪切,复制,粘贴
	connect(cutAction_,SIGNAL(triggered()),this,SLOT(cmdCut()));
	connect(copyAction_,SIGNAL(triggered()), this, SLOT(cmdCopy()));
	connect(pasteAction_,SIGNAL(triggered()), this, SLOT(cmdPaste()));
	connect(deleteAction_,SIGNAL(triggered()),this,SLOT(cmdDelete()));
}

void EditorMainWindow::OnNewScene()
{
	renameAction_->setEnabled(false);
	copyAction_->setEnabled(false);
	cutAction_->setEnabled(false);
	deleteAction_->setEnabled(false);

	updateUndoRedoActions();
}

//工具条上的按钮
void EditorMainWindow::CreateToolBars()
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

	//Select
	selectAction_ = new QAction(tr("Select"),this);
	selectAction_->setStatusTip(tr("Select"));
	selectAction_->setIcon(QIcon("Images/icons/Select.png"));
	selectAction_->setEnabled(true);
	selectAction_->setCheckable(true);
	selectAction_->setChecked(true);
	editToolBar->addAction(selectAction_);

	//Move
	moveAction_ = new QAction(tr("Move"),this);
	moveAction_->setStatusTip(tr("Move"));
	moveAction_->setIcon(QIcon("Images/icons/Move.png"));
	moveAction_->setEnabled(true);
	moveAction_->setCheckable(true);
	moveAction_->setChecked(false);
	editToolBar->addAction(moveAction_);

	//Rotate
	rotateAction_ = new QAction(tr("Rotate"),this);
	rotateAction_->setStatusTip(tr("Rotate"));
	rotateAction_->setIcon(QIcon("Images/icons/Rotate.png"));
	rotateAction_->setEnabled(true);
	rotateAction_->setCheckable(true);
	rotateAction_->setChecked(false);
	editToolBar->addAction(rotateAction_);

	editToolBar->addSeparator();
	editToolBar->addAction(renameAction_);
	editToolBar->addAction(cutAction_);
	editToolBar->addAction(copyAction_);
	editToolBar->addAction(pasteAction_);
	editToolBar->addSeparator();

	btnAttachTerrain = new QPushButton(QIcon("Images/Attach.png"),QString::fromLocal8Bit("吸附到地表"));
	editToolBar->addWidget(btnAttachTerrain);
	
	QPushButton* pTestEffect = new QPushButton(QIcon("Images/light.png"),QString::fromLocal8Bit("添加特效"));
	editToolBar->addWidget(pTestEffect);

	connect(selectAction_,SIGNAL(triggered(bool)), this,SLOT(sltSetToolSelect(bool)));
	connect(moveAction_,SIGNAL(triggered(bool)), this,SLOT(sltSetToolMove(bool)));
	connect(rotateAction_,SIGNAL(triggered(bool)), this,SLOT(sltSetToolRotate(bool)));

	connect(btnAttachTerrain,SIGNAL(clicked(bool)),this,SLOT(cmdAttachTerrain(bool)));
	connect(pTestEffect,SIGNAL(clicked(bool)),this,SLOT(cmdTestEffect(bool)));
}

void EditorMainWindow::cmdAttachTerrain(bool)
{
	EditorsRoot::Instance()->AttachSelectionsToTerrain();
}

void EditorMainWindow::cmdTestEffect(bool)
{
	ResourceCache* cache = GetSubsystem<ResourceCache>();

	//EditorsRoot::Instance()->AddEffetToSelectionNodes();
	RTTScene* rttScene = EditorsRoot::Instance()->GetRTTScene();

	Node* objNode = rttScene->CreateChildNode("testObj");
	StaticModel* pModel = objNode->CreateComponent<StaticModel>();
	pModel->SetModel(cache->GetResource<Model>("Models/Plane.mdl"));
	pModel->SetMaterial(cache->GetResource<Material>("Materials/StoneTiled.xml"));

	Image* image = rttScene->SaveJpg("D:/aa.png");
}

void EditorMainWindow::cmdModelTool()
{
	if(mModelTool == NULL)
		mModelTool = new ModelTool();

	mModelTool->show();
}

void EditorMainWindow::setApplicationObject(QObject* obj)
{
	obj->installEventFilter(this);
}

void EditorMainWindow::sltSetToolSelect(bool checked)
{
	EditorsRoot::Instance()->mSelectedTool = TOOL_SELECT;

	updateActions();

	EditorsRoot::Instance()->GetGizmo()->SetMode(enEditMode_Move);
}

void EditorMainWindow::sltSetToolMove(bool checked)
{
	EditorsRoot::Instance()->mSelectedTool = TOOL_MOVE;

	updateActions();

	EditorsRoot::Instance()->GetGizmo()->SetMode(enEditMode_Move);
}

void EditorMainWindow::sltSetToolRotate(bool checked)
{
	EditorsRoot::Instance()->mSelectedTool = TOOL_ROTATE;

	updateActions();

	EditorsRoot::Instance()->GetGizmo()->SetMode(enEditMode_Rotate);
}

void EditorMainWindow::cmdCut()
{
	EditorsRoot::Instance()->GetObjectPositionEditor()->OnNodesCut();
}

void EditorMainWindow::cmdCopy()
{
	EditorsRoot::Instance()->GetObjectPositionEditor()->OnNodesCopy();
}

void EditorMainWindow::cmdPaste()
{
	EditorsRoot::Instance()->GetObjectPositionEditor()->OnNodesPaste();
}

void EditorMainWindow::cmdDelete()
{
	int result = QMessageBox::question(this,"Question","Realy Delete?",QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Ok);
	switch(result)
	{
	case QMessageBox::Ok:
		EditorsRoot::Instance()->DeleteAllSelectionNodes();
		break;
	default:
		break;
	}
}

void EditorMainWindow::cmdUndo()
{
	UndoManager::Instance()->Undo();
	updateUndoRedoActions();
}

void EditorMainWindow::cmdRedo()
{
	UndoManager::Instance()->Redo();
	updateUndoRedoActions();
}

void EditorMainWindow::updateUndoRedoActions()
{
	undoAction_->setToolTip(UndoManager::Instance()->GetUndoString().c_str());
	undoAction_->setEnabled(UndoManager::Instance()->CanUndo());

	redoAction_->setToolTip(UndoManager::Instance()->GetRedoString().c_str());
	redoAction_->setEnabled(UndoManager::Instance()->CanRedo());
}

void EditorMainWindow::updateActions()
{
	selectAction_->setChecked(EditorsRoot::Instance()->mSelectedTool == TOOL_SELECT);
	moveAction_->setChecked(EditorsRoot::Instance()->mSelectedTool == TOOL_MOVE);
	rotateAction_->setChecked(EditorsRoot::Instance()->mSelectedTool == TOOL_ROTATE);

	EditorsRoot::Instance()->ActiveTool = EditorsRoot::Instance()->mSelectedTool;
}

void EditorMainWindow::onScreenshot()
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
	msg.setIconPixmap(QPixmap("Images/camera.png"));
	msg.exec();
}

void EditorMainWindow::AddLog(int type,const char* logContent)
{
	logView->AddLog(type,logContent);
}

void EditorMainWindow::onModeChanged(int index)
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