#include "stdafx.h"
#include "ObjectPositionEditor.h"
#include "../Manager/SceneHelper.h"
#include "DebugRenderer.h"
#include "../EditorSceneWidget.h"
#include "Skybox.h"

ObjectPositionEditor::ObjectPositionEditor(Context* context) : Object(context)
{
	CurrentHoverObject = NULL;
	isRectSelectionMode = false;

	SubscribeToEvent(E_POSTRENDERUPDATE,HANDLER(ObjectPositionEditor,HandlePostRenderUpdate));
}

//显示选中物体的包围盒
void ObjectPositionEditor::HandlePostRenderUpdate(StringHash eventType, VariantMap& eventData)
{
	EditorsRoot* pEditorRoot = EditorsRoot::Instance();
	DebugRenderer* debug = pEditorRoot->scene_->GetComponent<DebugRenderer>();
	if(debug == NULL)
		return;

	vector<Node*> nodes = pEditorRoot->GetUnionSelections();
	for(int i = 0;i < nodes.size();i ++)
	{
		Node* node = nodes[i];
		if(node->GetComponent<Skybox>() != NULL)
			continue;

		debug->AddNode(node,1.0f,false);

		const Vector<SharedPtr<Component> >& components = node->GetComponents();
		for(int j = 0;j < node->GetNumComponents();j ++)
		{
			Drawable* drawable = dynamic_cast<Drawable*>(components[j].Get());
			if(drawable != NULL)
			{
				debug->AddBoundingBox(drawable->GetWorldBoundingBox(),Color::WHITE,true);
			}
		}
	}

	//计算总的
	if(nodes.size() > 1)
	{
		BoundingBox allBox;
		for(int i = 0;i < nodes.size();i ++)
		{
			Node* node = nodes[i];
			if(node->GetComponent<Skybox>() != NULL)
				continue;

			const Vector<SharedPtr<Component> >& components = node->GetComponents();
			for(int j = 0;j < node->GetNumComponents();j ++)
			{
				Drawable* drawable = dynamic_cast<Drawable*>(components[j].Get());
				if(drawable != NULL)
				{
					allBox.Merge(drawable->GetWorldBoundingBox());
				}
			}
		}

		debug->AddBoundingBox(allBox,Color::BLUE,true);
	}

	if(CurrentHoverObject != NULL)
	{
		CurrentHoverObject->DrawDebugGeometry(debug,false);
	}
}

void ObjectPositionEditor::OnMouseLeftDown(float x,float y,unsigned int buttons)
{
	EditorsRoot* pEditorRoot = EditorsRoot::Instance();
	Editor3dGizmo* pGizmo = pEditorRoot->GetGizmo();

	//判断点击的内容，如果是物件判断有没有按住Ctrl多重选择
	IntVector2 pos(x,y);
	vector<SceneHitResult> result = SceneHelper::Instance()->QueryCurrentMousePosObjects(250.0,&pos);

	//判断是否是坐标轴
	if(is_axis_object(result) == true)
	{
		Graphics* graphics = GetSubsystem<Graphics>();
		Camera* camera = pEditorRoot->cameraNode_->GetComponent<Camera>();

		Ray cameraWorldRay = camera->GetScreenRay(x / graphics->GetWidth(),y / graphics->GetHeight());

		//判断并记录哪些坐标轴被选中了
		bool bXSelect;
		bool bYSelect;
		bool bZSelect;
		pGizmo->QuerySelectedStateByWorldRay(cameraWorldRay,bXSelect,bYSelect,bZSelect);
		pGizmo->gizmoAxisX->selected = bXSelect;
		pGizmo->gizmoAxisY->selected = bYSelect;
		pGizmo->gizmoAxisZ->selected = bZSelect;

		if(pGizmo->IsSelected())
		{
			pGizmo->BeginDrag();
		}

		//判断Ctrl有没有按下
		pGizmo->update_model();
	}
	else
	{
		Drawable* selObject = get_edit_object(result);
		if(selObject == NULL)
		{
			if(pEditorRoot->IsCtrlPressed() == false)
			{
				pEditorRoot->CancelAllSelection();
			}

			pEditorRoot->RectSelectionFrame_->Begin(x,y);

			isRectSelectionMode = true;
			rectSelectionStart.x_ = x;
			rectSelectionStart.y_ = y;
		}
		else
		{
			//选中的节点(后面要根据是否按住Ctrl来判断增加还是去除)
			Node* pSelNode = selObject->GetNode();
			pEditorRoot->OnNodeSelect(pSelNode);

			//
			pGizmo->gizmoAxisX->selected = true;
			pGizmo->gizmoAxisY->selected = true;
			pGizmo->gizmoAxisZ->selected = true;

			pGizmo->BeginDrag();
			pGizmo->update_model();
		}
	}
}

void ObjectPositionEditor::OnMouseLeftUp(float x,float y,unsigned int buttons)
{
	EditorsRoot* pEditorRoot = EditorsRoot::Instance();

	if(isRectSelectionMode == true)
	{
		pEditorRoot->OnRectSelectionEnd();
		isRectSelectionMode = false;
	}
	else
	{
		Editor3dGizmo* pGizmo = pEditorRoot->GetGizmo();
		if(pGizmo == NULL)
			return;

		if(pGizmo->IsDrag == true)
		{
			pGizmo->EndDrag();
		}
	}
}

Drawable* ObjectPositionEditor::get_edit_object(const vector<SceneHitResult>& result)
{
	for(int i = 0;i < result.size();i ++)
	{
		const SceneHitResult& obj = result[i];
		const Urho3D::String& typeName = obj.object->GetTypeName();
		if(typeName == "StaticModel" || typeName == "AnimatedModel")
		{
			if(obj.object != EditorsRoot::Instance()->GetGizmo()->GetModel())
			{
				return obj.object;
			}
		}
	}

	return NULL;
}

//第一个可视模型必须是坐标轴
bool ObjectPositionEditor::is_axis_object(const vector<SceneHitResult>& result)
{
	for(int i = 0;i < result.size();i ++)
	{
		const SceneHitResult& obj = result[i];
		const Urho3D::String& typeName = obj.object->GetTypeName();
		if(typeName == "StaticModel" || typeName == "AnimatedModel")
		{
			if(obj.object == EditorsRoot::Instance()->GetGizmo()->GetModel())
			{
				return true;
			}
		}
	}

	return false;
}

//1.框选模式 还是单选
//2.单选还是移动
void ObjectPositionEditor::OnMouseMove(float x,float y,unsigned int buttons)
{
	EditorsRoot* pEditorRoot = EditorsRoot::Instance();
	//显示名字UI
	char szName[256];

	//矩形框选模式
	if(isRectSelectionMode == true)
	{
		pEditorRoot->RectSelectionFrame_->UpdateShow(x,y);

		//遍历场景找到属于框内的对象
		pEditorRoot->RectSelectionNodes = SceneHelper::Instance()->QueryScreenRectNodes(pEditorRoot->RectSelectionFrame_->vecStart,pEditorRoot->RectSelectionFrame_->vecEnd);

		sprintf(szName,"Selected:%d",pEditorRoot->RectSelectionNodes.size());
		pEditorRoot->ObjectNameTip_->SetText(szName);
		pEditorRoot->ObjectNameTip_->UpdateShow(x,y);
	}
	else	//单选或者移动
	{
		Editor3dGizmo* pGizmo = pEditorRoot->GetGizmo();
		pEditorRoot->ObjectNameTip_->Hide();

		IntVector2 pos(x,y);
		vector<SceneHitResult> result = SceneHelper::Instance()->QueryCurrentMousePosObjects(250.0,&pos);

		Node* pHoverNode = NULL;
		Drawable* selObject = get_edit_object(result);
		if(selObject != NULL)
		{
			pHoverNode = selObject->GetNode();
		}

		//更新鼠标射线相对于坐标轴的情况(是否拖动都需要)
		Graphics* graphics = GetSubsystem<Graphics>();
		Camera* camera = pEditorRoot->cameraNode_->GetComponent<Camera>();

		Ray cameraWorldRay = camera->GetScreenRay(x / graphics->GetWidth(),y / graphics->GetHeight());

		//判断并记录哪些坐标轴被选中了
		bool bXHover;
		bool bYHover;
		bool bZHover;
		pGizmo->QuerySelectedStateByWorldRay(cameraWorldRay,bXHover,bYHover,bZHover);
		if(pGizmo->IsDrag == true)
		{
			pGizmo->UpdateMovePosition();
		}
		else
		{
			//当前落在坐标轴上
			if(is_axis_object(result) == true)
			{
				pGizmo->gizmoAxisX->hovered = bXHover;
				pGizmo->gizmoAxisY->hovered = bYHover;
				pGizmo->gizmoAxisZ->hovered = bZHover;
				pGizmo->update_model();

				if(bXHover || bYHover || bZHover)
				{
					EditorsRoot::Instance()->GetMainWindow()->setCursor(Qt::SizeAllCursor);
				}
				else
				{
					EditorsRoot::Instance()->GetMainWindow()->setCursor(Qt::ArrowCursor);
				}
			}
			else
			{
				//判断是否是选择的物体
				if(selObject == NULL)
				{
					//非当前可编辑的对象，直接退出
					EditorsRoot::Instance()->GetMainWindow()->setCursor(Qt::ArrowCursor);
					return;
				}

				sprintf(szName,"%s - %s",result[0].object->GetTypeName().CString(),result[0].object->GetNode()->GetName().CString());
				pEditorRoot->ObjectNameTip_->SetText(szName);
				pEditorRoot->ObjectNameTip_->UpdateShow(x,y);

				//当前悬浮的是选择的
				bool isHoverInSel = pEditorRoot->IsNodeInSelections(selObject->GetNode());

				//仅当在非当前选择的节点上时才显示+
				if(isHoverInSel == false)
				{
					EditorsRoot::Instance()->GetMainWindow()->setCursor(Qt::CrossCursor);
				}

				if(isHoverInSel == false && selObject == NULL)
				{
					EditorsRoot::Instance()->GetMainWindow()->setCursor(Qt::ArrowCursor);
				}
			}
		}
	}
}

bool ObjectPositionEditor::IsEditObject(Drawable* obj)
{
	const Urho3D::String& typeName = obj->GetTypeName();
	if(typeName == "StaticModel" || typeName == "AnimatedModel")
	{
		if(obj != EditorsRoot::Instance()->GetGizmo()->GetModel())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool ObjectPositionEditor::OnKeyDown(unsigned short key)
{
	return true;
}

bool ObjectPositionEditor::OnKeyUp(unsigned short key)
{
	if(key == 46)
	{
		vector<Node*>& editNodes = EditorsRoot::Instance()->SelectionNodes;
		for(int i = 0;i < editNodes.size();i ++)
		{
			Node* pNode = editNodes[i];
			if(pNode->GetParent())
			{
				pNode->GetParent()->RemoveChild(pNode);
			}
		}
	}

	return true;
}

void ObjectPositionEditor::OnObjectSelect(Node* pNode)
{
	EditorsRoot* pEditorsRoot = EditorsRoot::Instance();
	if(pEditorsRoot->IsCtrlPressed() == false)
	{
		pEditorsRoot->CancelAllSelection();
	}
	
	//判断有没有
	vector<Node*>& editNodes = pEditorsRoot->SelectionNodes;
	vector<Node*>::iterator it = editNodes.begin();
	while(it != editNodes.end())
	{
		if(*it == pNode)
		{
			editNodes.erase(it);
			return;
		}
		++ it;
	}

	editNodes.push_back(pNode);
}

void ObjectPositionEditor::OnNodesCut()
{
	OnNodesCopy();

	//todo:删除节点

}

void ObjectPositionEditor::OnNodesCopy()
{
	nodes_clipboard_data.clear();

	EditorsRoot* pEditorsRoot = EditorsRoot::Instance();
	const vector<Node*>& nodes = pEditorsRoot->SelectionNodes;
	for(int i = 0;i < nodes.size();i ++)
	{
		const Node* pNode = nodes[i];

		NodeData nodeData;
		nodeData.NodeID = pNode->GetID();
		nodeData.ParentNodeID = pNode->GetParent()->GetID();
		nodeData.xmlData = new XMLFile(pEditorsRoot->context_);
		pNode->SaveXML(nodeData.xmlData->CreateRoot("Node"));

		nodes_clipboard_data.push_back(nodeData);
	}
}

//粘贴以后剪贴板不释放
void ObjectPositionEditor::OnNodesPaste()
{
	EditorsRoot* pEditorsRoot = EditorsRoot::Instance();

	list<NodeData>::iterator it = nodes_clipboard_data.begin();
	while(it != nodes_clipboard_data.end())
	{
		NodeData& data = *it;

		Node* pParentNode = pEditorsRoot->scene_->GetNode(data.ParentNodeID);
		if(pParentNode != NULL)
		{
			Node* pNode = pParentNode->CreateChild();
			pNode->LoadXML(data.xmlData->GetRoot("Node"));
			pEditorsRoot->SendNodeUpdateShowEvent(pNode);
			//pNode->SetID(pEditorsRoot->scene_->GetFreeNodeID(CreateMode::REPLICATED));
			//pParentNode->AddChild(pNode);
			//pNode->SetName(String::EMPTY);

			//与源错开一点位置
			Vector3 vOldPos = pNode->GetWorldPosition();
			vOldPos += Vector3(1,0,1);
			pNode->SetWorldPosition(vOldPos);
		}

		++ it;
	}
}
