#include "stdafx.h"
#include "EditorsRoot.h"
#include "Skybox.h"
#include "../ObjectEditor/LightWidget.h"
#include "../ObjectEditor/SceneWidget.h"
#include "../ObjectEditor/SkyboxWidget.h"
#include "../ObjectEditor/StaticModelWidget.h"
#include "DebugRenderer.h"
#include <math.h>
#include <WinUser.h>
#include "../EditorAPI/EditorEvents.h"
#include "BillboardSet.h"
#include "ParticleEmitter.h"

EditorsRoot::EditorsRoot()
{
	yaw_ = 0.0f;
	pitch_ = 0.0f;

	engine_ = NULL;
	scene_ = NULL;
	cameraNode_ = NULL;

	terrain_ = NULL;
	terrainEditor_ = NULL;
	objectEditor_ = NULL;
	mainWindow_ = NULL;

	ActiveTool = TOOL_SELECT;

	pCurrentEditor = NULL;

	gizmo_ = NULL;

	ObjectNameTip_ = NULL;
	RectSelectionFrame_ = NULL;

	rttScene_ = NULL;
}

void EditorsRoot::InitEditors(Context* context)
{
	context_ = context;
	terrainEditor_ = new TerrainEditor(context);
	objectEditor_ = new ObjectPositionEditor(context);

	//默认是对象编辑工具
	pCurrentEditor = objectEditor_;
}

void EditorsRoot::InitEditorUI(Context* context)
{
	UIRoot_ = context->GetSubsystem<UI>()->GetRoot();

	ResourceCache* cache = context->GetSubsystem<ResourceCache>();
	XMLFile* style = cache->GetResource<XMLFile>("UI/DefaultStyle.xml");

	UIRoot_->SetDefaultStyle(style);

	ObjectNameTip_ = new ObjectNameTip();
	RectSelectionFrame_ = new RectSelectionFrame();

	//修改默认线宽
	glLineWidth(2);
}

bool EditorsRoot::IsNodeInSelections(const Node* pNode)
{
	for(int i = 0;i < SelectionNodes.size();i ++)
	{
		if(SelectionNodes[i] == pNode)
		{
			return true;
		}
	}

	return false;
}

float EditorsRoot::GetNodeSize(Node* pNode)
{
	Vector3 scale = pNode->GetWorldScale();

	if(pNode->GetComponent<StaticModel>() != NULL)
	{
		StaticModel* pModel = pNode->GetComponent<StaticModel>();
		if(pModel->GetModel())
		{
			const BoundingBox& box = pModel->GetModel()->GetBoundingBox();
			Vector3 vSize = box.Size() * scale;
			return max(max(vSize.x_,vSize.y_),vSize.z_) * 2.0f;
		}
	}
	else if(pNode->GetComponent<AnimatedModel>() != NULL)
	{
		AnimatedModel* pModel = pNode->GetComponent<AnimatedModel>();
		if(pModel->GetModel())
		{
			const BoundingBox& box = pModel->GetModel()->GetBoundingBox();
			Vector3 vSize = box.Size() * scale;
			return max(max(vSize.x_,vSize.y_),vSize.z_) * 2.0f;
		}
	}

	return 0;
}

void EditorsRoot::OpenScene(const char* sceneFile)
{

}

void EditorsRoot::CancelAllSelection()
{
	SelectionNodes.clear();
	RectSelectionNodes.clear();

	SendNodeSelectionChangeEvent();
}

void EditorsRoot::SendNodeSelectionChangeEvent()
{
	scene_->SendEvent(E_NODE_SELECTION_CHANGE);
}

void EditorsRoot::SendNodeUpdateShowEvent(const Node* pNode)
{
	VariantMap eventData;
	eventData[NodeUpdateShow::P_NODE] = pNode;

	scene_->SendEvent(E_NODE_UPDATE_SHOW,eventData);
}

void EditorsRoot::OnRectSelectionEnd()
{
	RectSelectionFrame_->Hide();

	//合并成最终的选择
	SelectionNodes = GetUnionSelections();
	RectSelectionNodes.clear();

	gizmo_->GetNode()->SetEnabled(false);
	SendNodeSelectionChangeEvent();
}

vector<Node*> EditorsRoot::GetUnionSelections()
{
	//交集(防止重复)
	vector<Node*> intersect;
	for(int i = 0;i < RectSelectionNodes.size();i ++)
	{
		if(IsNodeInSelections(RectSelectionNodes[i]) == true)
		{
			intersect.push_back(RectSelectionNodes[i]);
		}
	}

	//并集(防止重复)
	vector<Node*> unionset = SelectionNodes;
	for(int i = 0;i < RectSelectionNodes.size();i ++)
	{
		if(IsNodeInSelections(RectSelectionNodes[i]) == false)
		{
			unionset.push_back(RectSelectionNodes[i]);
		}
	}

	//从并集中删除交集部分
	for(int i = 0;i < intersect.size();i ++)
	{
		vector<Node*>::iterator it = unionset.begin();
		while(it != unionset.end())
		{
			if(*it == intersect[i])
			{
				unionset.erase(it);
				break;
			}
			++ it;
		}
	}

	return unionset;
}

RTTScene* EditorsRoot::GetRTTScene()
{
	if(rttScene_ == NULL)
	{
		rttScene_ = new RTTScene(context_);
	}

	return rttScene_;
}

// 显示编辑面板，记录当前编辑物件
void EditorsRoot::OnNodeSelect(Node* pNode)
{
	GetMainWindow()->GetPropertiesView()->SetTarget(pNode);

	//显示Gizmo
	GetGizmo()->ShowGizmo();
	GetGizmo()->GetNode()->SetWorldPosition(pNode->GetWorldPosition());

	//根据选择对象的类型决定 1.显示什么面板 2.设置当前的Editor
	if(pNode->GetComponent<TerrainPatch>() != NULL || pNode->GetComponent<Terrain>() != NULL)
	{
		pCurrentEditor = terrainEditor_;
		//显示Terrain工具
	}
	else
	{
		pCurrentEditor = objectEditor_;
	}

	if(pCurrentEditor != NULL)
	{
		pCurrentEditor->OnObjectSelect(pNode);

		SendNodeSelectionChangeEvent();
	}
}

void EditorsRoot::DeleteAllSelectionNodes()
{
	//防止节点的嵌套关系，Node*失效
	vector<int> vIds;
	for(int i = 0;i < SelectionNodes.size();i ++)
	{
		vIds.push_back(SelectionNodes[i]->GetID());
	}

	for(int i = 0;i < vIds.size();i ++)
	{
		int ID = vIds[i];
		Node* pNode = scene_->GetNode(ID);
		if(pNode != NULL)
		{
			pNode->Remove();
		}
	}

	SelectionNodes.clear();
}

void EditorsRoot::AttachSelectionsToTerrain()
{
	if(terrain_ == NULL)
		return;

	for(int i = 0;i < SelectionNodes.size();i ++)
	{
		Node* pNode = SelectionNodes[i];
		Vector3 wPos = pNode->GetWorldPosition();
		float height = terrain_->GetHeight(wPos);
		wPos.y_ = height + terrain_->GetNode()->GetWorldPosition().y_;

		pNode->SetWorldPosition(wPos);
	}
}

void EditorsRoot::AddEffetToSelectionNodes()
{
	// Create billboard sets (floating smoke)
	const unsigned NUM_BILLBOARDNODES = 25;
	const unsigned NUM_BILLBOARDS = 10;
	ResourceCache* cache = scene_->GetSubsystem<ResourceCache>();

	for(int i = 0;i < SelectionNodes.size();i ++)
	{
		Node* pNode = SelectionNodes[i];
		ParticleEmitter* particleEmitter = pNode->CreateComponent<ParticleEmitter>();
		//particleEmitter->Load(cache->GetResource<XMLFile>("Particle/Smoke.xml"));
		//particleEmitter->SetParticleSize(Vector2(3,3));
		//particleEmitter->SetColor(Color(0,1,0));
		//particleEmitter->SetEmitterSize(Vector3(10,10,10));
		//particleEmitter->

		//BillboardSet* billboardObject = pNode->CreateComponent<BillboardSet>();
		//billboardObject->SetNumBillboards(NUM_BILLBOARDS);
		//billboardObject->SetMaterial(cache->GetResource<Material>("Materials/LitSmoke.xml"));
		//billboardObject->SetSorted(true);

		//for (unsigned j = 0; j < NUM_BILLBOARDS; ++j)
		//{
		//	Billboard* bb = billboardObject->GetBillboard(j);
		//	bb->position_ = Vector3(Random(12.0f) - 6.0f, Random(8.0f) - 4.0f, Random(12.0f) - 6.0f);
		//	bb->size_ = Vector2(Random(10.0f) + 3.0f, Random(10.0f) + 3.0f);
		//	bb->rotation_ = Random() * 360.0f;
		//	bb->enabled_ = true;
		//}

		//// After modifying the billboards, they need to be "commited" so that the BillboardSet updates its internals
		//billboardObject->Commit();
	}
}

bool EditorsRoot::IsCtrlPressed()
{
	if(::GetKeyState(VK_CONTROL) < 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

IntVector2 EditorsRoot::GetScreenMousePos()
{
	Input* input = context_->GetSubsystem<Input>();
	UI* ui = context_->GetSubsystem<UI>();
	IntVector2 pos = ui->GetCursorPosition();

	return pos;
}

void EditorsRoot::OnKeyDown(unsigned int key)
{
	if(pCurrentEditor)
	{
		pCurrentEditor->OnKeyDown(key);
	}
}

void EditorsRoot::OnKeyUp(unsigned int key)
{
	if(pCurrentEditor)
	{
		pCurrentEditor->OnKeyUp(key);
	}
}

void EditorsRoot::OnMouseMove(float x,float y,unsigned int buttons)
{
	if(pCurrentEditor)
	{
		pCurrentEditor->OnMouseMove(x,y,buttons);
	}
}

void EditorsRoot::OnMouseLeftDown(float x,float y,unsigned int buttons)
{
	if(pCurrentEditor)
	{
		pCurrentEditor->OnMouseLeftDown(x,y,buttons);
	}
}

void EditorsRoot::OnMouseLeftUp(float x,float y,unsigned int buttons)
{
	if(pCurrentEditor)
	{
		pCurrentEditor->OnMouseLeftUp(x,y,buttons);
	}
}

void EditorsRoot::OnMouseRightDown(float x,float y,unsigned int buttons)
{
	if(pCurrentEditor)
	{
		pCurrentEditor->OnMouseRightDown(x,y,buttons);
	}
}

void EditorsRoot::OnMouseRightUp(float x,float y,unsigned int buttons)
{
	if(pCurrentEditor)
	{
		pCurrentEditor->OnMouseRightUp(x,y,buttons);
	}
}

void EditorsRoot::OnMouseMiddleDown(float x,float y,unsigned int buttons)
{

}

void EditorsRoot::OnMouseMiddleUp(float x,float y,unsigned int buttons)
{

}

void EditorsRoot::OnMouseWheel(float x,float y,unsigned int buttons)
{
	Input* input = context_->GetSubsystem<Input>();
	cameraNode_->Translate(Vector3::FORWARD * input->GetMouseMoveWheel());
}
