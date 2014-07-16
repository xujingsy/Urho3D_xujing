#pragma once
#include "Misc/Single.h"
#include "../EditorMainWindow.h"
#include "../Terrain/TerrainEditor.h"
#include "Context.h"
#include <string>
#include <map>

using namespace std;
#include "../EditorAssist/RTTScene.h"
#include "../EditorMainWindow.h"
#include "../EditorAssist/EditorGizmo.h"
#include "../ObjectEditor/ObjectPositionEditor.h"
#include "../EditorAssist/ObjectNameTip.h"
#include "../EditorAssist/RectSelectionFrame.h"

namespace Urho3D
{
	class Engine;
	class Node;
	class Scene;
	class Terrain;
	class IntVector2;
}

enum enEditorTools
{
	TOOL_SELECT = 0,
	TOOL_MOVE,
	TOOL_ROTATE,
	TOOL_SCALE,
	TOOL_DEFORM,
	TOOL_SMOOTH,
	TOOL_PAINT,	//刷地表
};

//编辑器底层操作的接口，供各个插件使用
//编辑器如果使用对应的事件，必须先注册
class EditorsRoot : public Singleton<EditorsRoot>
{
public:
	EditorsRoot();

	//添加新的ToolBar
	void RegisterToolBar(void* plugin,void * toolbar);

	//注册响应的事件
	void RegisterMouseHandler(void* source);

	void InitEditors(Context* context);

	//初始化编辑器本身需要的UI
	void InitEditorUI(Context* context);

	Context* context_;

	TerrainEditor* GetTerrainEditor()
	{
		return terrainEditor_;
	}

	ObjectPositionEditor* GetObjectPositionEditor()
	{
		return objectEditor_;
	}

	IntVector2 GetScreenMousePos();

	//引擎的一些全局变量
	Engine* engine_;
	Scene* scene_;
	Node* cameraNode_;
	/// Camera yaw angle.
	float yaw_;
	/// Camera pitch angle.
	float pitch_;

	Node* terrainNode;
	Terrain* terrain_;
	UIElement* UIRoot_;
	ObjectNameTip* ObjectNameTip_;
	RectSelectionFrame* RectSelectionFrame_;

	//当前活跃的编辑方式
	enEditorTools ActiveTool;

	EditorMainWindow* GetMainWindow()
	{
		return mainWindow_;
	}

	void SetMainWindow(EditorMainWindow* pWindow)
	{
		mainWindow_ = pWindow;
	}

	Editor3dGizmo* GetGizmo()
	{
		if(gizmo_ == NULL)
		{
			gizmo_ = new Editor3dGizmo(context_);
			gizmo_->CreateGizmo();
		}

		return gizmo_;
	}

	//返回Node的评估大小，供Camera定位观察使用
	float GetNodeSize(Node* pNode);

	void CancelAllSelection();

	//发送节点选择变化消息
	void SendNodeSelectionChangeEvent();

	//某节点显示更新
	void SendNodeUpdateShowEvent(const Node* pNode);

	//当框选结束
	void OnRectSelectionEnd();

	//Node是否在选择范围内
	bool IsNodeInSelections(const Node* pNode);

	//删除所有选择的节点
	void DeleteAllSelectionNodes();

	//将所选择物件紧贴地表
	void AttachSelectionsToTerrain();

	//测试添加特效
	void AddEffetToSelectionNodes();

	vector<Node*> GetUnionSelections();

	RTTScene* GetRTTScene();
public:
	// 事件接口
	//1.拖动
	bool OnDragEnter(void* source);
	bool OnDragMove(void* source,unsigned int modifier,int x,int y);
	void OnDragLeave();

	//2.键盘
	void OnKeyDown(unsigned int key);
	void OnKeyUp(unsigned int key);
	
	//3.Mouse
	void OnMouseMove(float x,float y,unsigned int buttons);
	void OnMouseLeftDown(float x,float y,unsigned int buttons);
	void OnMouseLeftUp(float x,float y,unsigned int buttons);
	void OnMouseRightDown(float x,float y,unsigned int buttons);
	void OnMouseRightUp(float x,float y,unsigned int buttons);
	void OnMouseMiddleDown(float x,float y,unsigned int buttons);
	void OnMouseMiddleUp(float x,float y,unsigned int buttons);
	void OnMouseWheel(float x,float y,unsigned int buttons);

	void OnNodeSelect(Node* pNode);

	//当前Ctrl是否按下
	bool IsCtrlPressed();

	enEditorTools mSelectedTool;

	vector<Node*> SelectionNodes;		//当前选中的节点
	vector<Node*> RectSelectionNodes;	//框选节点集合
private:
	BaseEditor* pCurrentEditor;
	//todo:是否可以通用化？
	TerrainEditor* terrainEditor_;
	ObjectPositionEditor* objectEditor_;

	Editor3dGizmo* gizmo_;
	EditorMainWindow* mainWindow_;

	RTTScene* rttScene_;
};
