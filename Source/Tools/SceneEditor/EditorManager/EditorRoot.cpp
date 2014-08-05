#include "stdafx.h"
#include <math.h>

#include "EditorRoot.h"
#include "Skybox.h"
#include "AnimationState.h"
#include "Animation.h"
#include "DebugRenderer.h"
#include "BillboardSet.h"
#include "ParticleEmitter.h"
#include "SceneEvents.h"
#include "Window.h"
#include "../Terrain/Mover.h"

#include "../ComponentEditor/LightWidget.h"
#include "../ComponentEditor/SceneWidget.h"
#include "../ComponentEditor/SkyboxWidget.h"
#include "../ComponentEditor/StaticModelWidget.h"
#include "../EditorManager/EditorEvents.h"

//todo:
#include <WinUser.h>

EditorRoot::EditorRoot()
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

EditorRoot::~EditorRoot()
{

}

void EditorRoot::NewScene()
{
	if(scene_)
	{
		scene_->Remove();
	}
	else
	{
		scene_ = new Scene(context_);
	}

	ResourceCache* cache = context_->GetSubsystem<ResourceCache>();

	scene_->CreateComponent<DebugRenderer>();
	scene_->CreateComponent<Octree>();

	//创建默认Zone
	Node* zoneNode = scene_->CreateChild("Zone");
	Zone* zone = zoneNode->CreateComponent<Zone>();
	zone->SetBoundingBox(BoundingBox(-1000.0f, 1000.0f));
	zone->SetAmbientColor(Color(250./255.,208./255.,160./255.));
	zone->SetOccluder(true);
	zone->SetAmbientGradient(true);
	zone->SetFogColor(Color(0.1f, 0.2f, 0.3f));
	zone->SetFogStart(10.0f);
	zone->SetFogEnd(100.0f);

	//创建默认的天空盒
	Node* skyNode = scene_->CreateChild("Sky");
	skyNode->SetScale(500.0f); // The scale actually does not matter
	Skybox* skybox = skyNode->CreateComponent<Skybox>();
	skybox->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
	skybox->SetMaterial(cache->GetResource<Material>("Materials/Skybox.xml"));

	// 创建默认地形
	terrainNode = scene_->CreateChild("Terrain");
	terrainNode->SetPosition(Vector3::ZERO);
	terrain_ = terrainNode->CreateComponent<Terrain>();
	terrain_->SetPatchSize(64);
	terrain_->SetSpacing(Vector3(2.0f, 0.1f, 2.0f)); // Spacing between vertices and vertical resolution of the height map
	terrain_->SetSmoothing(false);
	terrain_->SetHeightMap(cache->GetResource<Image>("Textures/HeightMap.png"));
	terrain_->SetMaterial(cache->GetResource<Material>("Materials/Terrain.xml"));
	// The terrain consists of large triangles, which fits well for occlusion rendering, as a hill can occlude all
	// terrain patches and other objects behind it
	terrain_->SetOccluder(true);

	//创建默认灯光
	Node* lightNode = scene_->CreateChild("DirectionalLight");
	lightNode->SetDirection(Vector3(1.6f, -1.0f, 0.8f)); // The direction vector does not need to be normalized
	Light* light = lightNode->CreateComponent<Light>();
	light->SetLightType(LIGHT_DIRECTIONAL);
	float fBri = light->GetBrightness();
	light->SetBrightness(1.0f);
	light->SetCastShadows(true);
	//light->SetShadowBias(BiasParameters(0.00025f, 0.5f));
	light->SetShadowCascade(CascadeParameters(10.0f, 50.0f, 200.0f, 0.0f, 0.8f));
	light->SetShadowIntensity(1.0);

	//创建主Camera
	cameraNode_ = scene_->CreateChild("Camera");
	cameraNode_->CreateComponent<Camera>();
	cameraNode_->SetPosition(Vector3(0.0f, 15.0f, 0.0f));	//初始位置

	Camera* pCamera = cameraNode_->GetComponent<Camera>();
	pCamera->SetFarClip(300.0f);

	//设置默认ViewPort
	SharedPtr<Viewport> viewport(new Viewport(context_, scene_, cameraNode_->GetComponent<Camera>()));
	Renderer* renderer = context_->GetSubsystem<Renderer>();
	renderer->SetViewport(0, viewport);

	const unsigned NUM_OBJECTS = 200;
	for (unsigned i = 0; i < NUM_OBJECTS; ++i)
	{
		Node* mushroomNode = scene_->CreateChild("Mushroom");
		mushroomNode->SetPosition(Vector3(Random(90.0f) - 45.0f, 0.0f, Random(90.0f) - 45.0f));
		mushroomNode->SetRotation(Quaternion(0.0f, Random(360.0f), 0.0f));
		mushroomNode->SetScale(0.5f + Random(2.0f));
		StaticModel* mushroomObject = mushroomNode->CreateComponent<StaticModel>();
		mushroomObject->SetModel(cache->GetResource<Model>("Models/Mushroom.mdl"));
		mushroomObject->SetMaterial(cache->GetResource<Material>("Materials/Mushroom.xml"));
	}

	// Create animated models
	const unsigned NUM_MODELS = 100;
	const float MODEL_MOVE_SPEED = 2.0f;
	const float MODEL_ROTATE_SPEED = 100.0f;
	const BoundingBox bounds(Vector3(-47.0f, 0.0f, -47.0f), Vector3(47.0f, 0.0f, 47.0f));

	for (unsigned i = 0; i < NUM_MODELS; ++i)
	{
		Node* modelNode = scene_->CreateChild("Jack");
		modelNode->SetPosition(Vector3(Random(90.0f) - 45.0f, 0.0f, Random(90.0f) - 45.0f));
		modelNode->SetRotation(Quaternion(0.0f, Random(360.0f), 0.0f));
		AnimatedModel* modelObject = modelNode->CreateComponent<AnimatedModel>();
		modelObject->SetModel(cache->GetResource<Model>("Models/Jack.mdl"));
		modelObject->SetMaterial(cache->GetResource<Material>("Materials/Jack.xml"));
		modelObject->SetCastShadows(true);

		// Create an AnimationState for a walk animation. Its time position will need to be manually updated to advance the
		// animation, The alternative would be to use an AnimationController component which updates the animation automatically,
		// but we need to update the model's position manually in any case
		Animation* walkAnimation = cache->GetResource<Animation>("Models/Jack_Walk.ani");
		AnimationState* state = modelObject->AddAnimationState(walkAnimation);
		// The state would fail to create (return null) if the animation was not found
		if (state)
		{
			// Enable full blending weight and looping
			state->SetWeight(1.0f);
			state->SetLooped(true);
		}

		// Create our custom Mover component that will move & animate the model during each frame's update
		Mover* mover = modelNode->CreateComponent<Mover>();
		mover->SetParameters(MODEL_MOVE_SPEED, MODEL_ROTATE_SPEED, bounds);
	}

	//////////////////////////////////////////////////////////////////////////
	OnSceneReset();
	fileName_.Clear();
}

bool EditorRoot::OpenScene(const char* sceneFile)
{
	if(scene_)
	{
		scene_->Remove();
	}
	else
	{
		scene_ = new Scene(context_);
	}

	File file(context_);
	if(!file.Open(sceneFile, FILE_READ))
	{
		LOGERROR("Open " + String(sceneFile) + " failed");
		return false;
	}

	bool result = scene_->LoadXML(file);

	if(result == true)
	{
		scene_->SendEvent(E_SCENE_RESET);
	}

	return result;
}

void EditorRoot::SaveScene(const char* sceneFile)
{
	File file(context_);
	if(!file.Open(sceneFile, FILE_WRITE))
	{
		LOGERROR("Open " + String(sceneFile) + " failed");
		return;
	}

	scene_->SaveXML(file);
}

void EditorRoot::OnSceneReset()
{
	//选择物件时显示的坐标轴
	gizmo_ = new Editor3dGizmo(context_);
	gizmo_->CreateGizmo();

	terrainEditor_->OnSceneReset();
}

void EditorRoot::InitEditors(Context* context)
{
	context_ = context;
	terrainEditor_ = new TerrainEditor(context);
	objectEditor_ = new ObjectPositionEditor(context);

	//默认是对象编辑工具
	pCurrentEditor = objectEditor_;
}

void EditorRoot::InitEditorUI(Context* context)
{
	UIRoot_ = context->GetSubsystem<UI>()->GetRoot();

	Window* window_ = new Window(context);
	UIRoot_->AddChild(window_);
	window_->SetMinSize(150, 155);
	window_->SetLayout(LM_VERTICAL, 6, IntRect(6, 6, 6, 6));
	window_->SetAlignment(HA_RIGHT, VA_TOP);
	window_->SetName("Window");

	ResourceCache* cache = context->GetSubsystem<ResourceCache>();
	XMLFile* style = cache->GetResource<XMLFile>("UI/DefaultStyle.xml");

	UIRoot_->SetDefaultStyle(style);

	//地形UI
	//window_->AddChild(context->GetSubsystem<UI>()->LoadLayout(cache->GetResource<XMLFile>("UI/TerrainEditorToolBox.xml")));

	ObjectNameTip_ = new ObjectNameTip();
	RectSelectionFrame_ = new RectSelectionFrame();

	//修改默认线宽
	glLineWidth(2);
}

bool EditorRoot::IsNodeInSelections(const Node* pNode)
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

float EditorRoot::GetNodeSize(Node* pNode)
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

void EditorRoot::CancelAllSelection()
{
	SelectionNodes.clear();
	RectSelectionNodes.clear();

	SendNodeSelectionChangeEvent();
}

void EditorRoot::SendNodeSelectionChangeEvent()
{
	scene_->SendEvent(E_NODE_SELECTION_CHANGE);
}

void EditorRoot::SendNodeUpdateShowEvent(const Node* pNode)
{
	VariantMap eventData;
	eventData[NodeUpdateShow::P_NODE] = pNode;

	scene_->SendEvent(E_NODE_UPDATE_SHOW,eventData);
}

void EditorRoot::OnRectSelectionEnd()
{
	RectSelectionFrame_->Hide();

	//合并成最终的选择
	SelectionNodes = GetUnionSelections();
	RectSelectionNodes.clear();

	gizmo_->GetNode()->SetEnabled(false);
	SendNodeSelectionChangeEvent();
}

vector<Node*> EditorRoot::GetUnionSelections()
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

RTTScene* EditorRoot::GetRTTScene()
{
	if(rttScene_ == NULL)
	{
		rttScene_ = new RTTScene(context_);
	}

	return rttScene_;
}

// 显示编辑面板，记录当前编辑物件
void EditorRoot::OnNodeSelect(Node* pNode)
{
	GetMainWindow()->GetPropertiesView()->SetEditNode(pNode);

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

void EditorRoot::DeleteAllSelectionNodes()
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
			using namespace NodeRemoved;

			VariantMap eventData;
			eventData[NodeRemoved::P_NODE] = pNode;

			scene_->SendEvent(E_NODEREMOVED,eventData);

			pNode->Remove();
		}
	}

	SelectionNodes.clear();
}

void EditorRoot::AttachSelectionsToTerrain()
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

void EditorRoot::AddEffetToSelectionNodes()
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

bool EditorRoot::IsCtrlPressed()
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

IntVector2 EditorRoot::GetScreenMousePos()
{
	Input* input = context_->GetSubsystem<Input>();
	UI* ui = context_->GetSubsystem<UI>();
	IntVector2 pos = ui->GetCursorPosition();

	return pos;
}

void EditorRoot::OnKeyDown(unsigned int key)
{
	if(pCurrentEditor)
	{
		pCurrentEditor->OnKeyDown(key);
	}
}

void EditorRoot::OnKeyUp(unsigned int key)
{
	if(pCurrentEditor)
	{
		pCurrentEditor->OnKeyUp(key);
	}
}

void EditorRoot::OnMouseMove(float x,float y,unsigned int buttons)
{
	if(pCurrentEditor)
	{
		pCurrentEditor->OnMouseMove(x,y,buttons);
	}
}

void EditorRoot::OnMouseLeftDown(float x,float y,unsigned int buttons)
{
	if(pCurrentEditor)
	{
		pCurrentEditor->OnMouseLeftDown(x,y,buttons);
	}
}

void EditorRoot::OnMouseLeftUp(float x,float y,unsigned int buttons)
{
	if(pCurrentEditor)
	{
		pCurrentEditor->OnMouseLeftUp(x,y,buttons);
	}
}

void EditorRoot::OnMouseRightDown(float x,float y,unsigned int buttons)
{
	if(pCurrentEditor)
	{
		pCurrentEditor->OnMouseRightDown(x,y,buttons);
	}
}

void EditorRoot::OnMouseRightUp(float x,float y,unsigned int buttons)
{
	if(pCurrentEditor)
	{
		pCurrentEditor->OnMouseRightUp(x,y,buttons);
	}
}

void EditorRoot::OnMouseMiddleDown(float x,float y,unsigned int buttons)
{

}

void EditorRoot::OnMouseMiddleUp(float x,float y,unsigned int buttons)
{

}

void EditorRoot::OnMouseWheel(float x,float y,unsigned int buttons)
{
	Camera* camera = cameraNode_->GetComponent<Camera>();

	Input* input = context_->GetSubsystem<Input>();
	float dWheel = (float)input->GetMouseMoveWheel() + 0.2f;
	float zoom = camera->GetZoom() * (1.0f + dWheel);
	if (zoom < 0.5f)
		zoom = 0.5f;
	if(zoom > 3.0f)
		zoom = 3.0f;

	camera->SetZoom(zoom);
}
