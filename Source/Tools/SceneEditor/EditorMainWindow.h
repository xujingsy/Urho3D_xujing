#pragma once
#include <QMainWindow.h>
#include <qlistwidget.h>
#include <qtabwidget.h>
#include <qdockwidget.h>
#include "EditorWindow/SceneView.h"
#include "EditorWindow/ObjectPropertiesView.h"
#include "EditorWindow/ResExplorerView.h"
#include "OtherTools/ModelTool.h"
#include "EditorWindow/LogView.h"
#include "Object.h"
using namespace Urho3D;

class QAction;
class QTreeWidget;
class EditorSceneWidget;

/// 整个地编的主窗口，包含项目栏，资源栏，渲染窗口，输出窗口等
class EditorMainWindow : public QMainWindow , public Object
{
    Q_OBJECT;
	OBJECT(EditorMainWindow);
public:
    EditorMainWindow(QWidget *parent=0, Qt::WindowFlags flags =0);
    virtual ~EditorMainWindow();

	EditorSceneWidget* GetSceneWidget()
	{
		return pSceneWidget;
	}

	SceneView* GetProjectView()
	{
		return treeProject;
	}

	void AddLog(int type,const char* logContent);

	//更新按钮的显示状态
	void updateActions();
	void updateUndoRedoActions();

	QDockWidget* GetEditorPanel()
	{
		return dockRight;
	}

	ObjectPropertiesView* GetPropertiesView()
	{
		return propertyView;
	}

	void setApplicationObject(QObject* obj);

	//public的菜单Action，用于右键菜单
	QAction* renameAction_;
	QAction* copyAction_;
	QAction* cutAction_;
	QAction* pasteAction_;
	QAction* deleteAction_;
private:
	void CreateActions();
    void CreateMenuBar();

	void CreateToolBars();
    void OnNewScene();

	//文件工具条
	QToolBar* fileToolBar;
	QToolBar* editToolBar;

    QAction* openAction_;
    QAction* saveAction_;
	QAction* exitAction_;

	QAction* undoAction_;
	QAction* redoAction_;
	QAction* screenshot_;

	QAction* selectAction_;
	QAction* moveAction_;
	QAction* rotateAction_;

	QAction* modelTransAction_;

	QAction* helpAction_;
	QAction* aboutAction_;


	//Dock窗口
	QDockWidget* dockLeft;
	QDockWidget* dockRight;
	QDockWidget* dockBottom;
	QDockWidget* dockCenter;

	QPushButton* btnAttachTerrain;

	// 项目资源目录，存放当前场景的所有资源的目录树
	SceneView* treeProject;

	ResExplorerView* resTreeView;
	// 
	ObjectPropertiesView* propertyView;

	QTabWidget* rightTabView;

	LogView* logView;

	// 场景渲染窗口
	EditorSceneWidget* pSceneWidget;

	ModelTool* mModelTool;

	bool eventFilter(QObject *obj,  QEvent *event);

	void HandleSelectionChanged(StringHash eventType, VariantMap& eventData);
private Q_SLOTS:
	void onModeChanged(int index);
	void HandleScreenshotAction();

	void HandleUndoAction();
	void HandleRedoAction();

	//模型工具
	void HandleModelToolAction();

	void HandleSelectTool(bool checked);
	void HandleMoveTool(bool checked);
	void HandleRotateTool(bool checked);

	void HandleOpenAction();
	void HandleSaveAction();

	void HandleCutAction();
	void HandleCopyAction();
	void HandlePasteAction();
	void HandleDeleteAction();
	void HandleAttachTerrainAction(bool);
	void HandleTestEffectAction(bool);

	//打开关于对话框
	void HandleAbout();
};
