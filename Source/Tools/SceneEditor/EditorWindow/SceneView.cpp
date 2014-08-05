#include "stdafx.h"
#include "SceneView.h"
#include "Skybox.h"
#include "SceneEvents.h"
using namespace Urho3D;
#include "../EditorManager/EditorEvents.h"
#include "../EditorSceneWidget.h"

SceneView::SceneView(QWidget* parent,Context* context) : QTreeWidget(parent) , Object(context)
{
	sceneRootNode_ = NULL;
	

	//设置树显示的列数
	setColumnCount(1);
	setHeaderHidden(true);
	setSelectionMode(QAbstractItemView::ExtendedSelection);
	setAutoScroll(true);

	bSelectTrigger = true;

	connect(this,SIGNAL(itemSelectionChanged()),this,SLOT(selectionChanged()));
	connect(this,SIGNAL(itemDoubleClicked(QTreeWidgetItem *,int)),this,SLOT(itemDoubleClicked(QTreeWidgetItem *,int)));
}

void SceneView::mouseReleaseEvent(QMouseEvent *evt)
{
	if(evt->button() == Qt::RightButton)
	{
		EditorRoot::Instance()->GetMainWindow()->GetSceneWidget()->showObjectMenu();
	}
}

void SceneView::ClearAllSelection()
{
	bSelectTrigger = false;

	QTreeWidgetItemIterator it(this);
	while(*it)
	{
		QTreeWidgetItem* item = *it;
		if(item->isSelected())
		{
			item->setSelected(false);
		}

		++ it;
	}

	bSelectTrigger = true;
}

void SceneView::HandleNodeAdded(StringHash eventType, VariantMap& eventData)
{
	Node* parentNode = (Node*)eventData[NodeAdded::P_PARENT].GetPtr();
	Node* node = (Node*)eventData[NodeAdded::P_NODE].GetPtr();

	QTreeWidgetItem* parentItem = get_item_by_node(parentNode);
	if(parentNode != NULL)
	{
		add_node_to_tree(parentItem,node);
	}
}

void SceneView::HandleNodeRemoved(StringHash eventType, VariantMap& eventData)
{
	Node* parentNode = (Node*)eventData[NodeRemoved::P_PARENT].GetPtr();
	Node* node = (Node*)eventData[NodeRemoved::P_NODE].GetPtr();

	QTreeWidgetItem* item = get_item_by_node(node);

	this->removeItemWidget(item,0);
}

void SceneView::HandleNodeSelectionChange(StringHash eventType, VariantMap& eventData)
{
	ClearAllSelection();

	bSelectTrigger = false;
	vector<Node*>& SelectionNodes = EditorRoot::Instance()->SelectionNodes;
	for(int i = 0;i < SelectionNodes.size();i ++)
	{
		QTreeWidgetItem* item = get_item_by_node(SelectionNodes[i]);
		item->setSelected(true);
	}

	bSelectTrigger = true;
}

void SceneView::HandleNodeUpdateShow(StringHash eventType, VariantMap& eventData)
{
	Node* pNode = (Node*)eventData[NodeUpdateShow::P_NODE].GetPtr();
	QTreeWidgetItem* item = get_item_by_node(pNode);
	if(item != NULL)
	{
		update_node_icon(item,pNode);
	}
}

void SceneView::Init(Node* pSceneNode)
{
	context_ = pSceneNode->GetContext();

	add_node_to_tree(sceneRootNode_,pSceneNode);

	if(sceneRootNode_)
	{
		sceneRootNode_->setExpanded(true);
	}

	//节点增删
	SubscribeToEvent(E_NODEADDED,HANDLER(SceneView,HandleNodeAdded));
	SubscribeToEvent(E_NODEREMOVED,HANDLER(SceneView,HandleNodeRemoved));

	//节点选择的变化
	SubscribeToEvent(E_NODE_SELECTION_CHANGE,HANDLER(SceneView,HandleNodeSelectionChange));
	SubscribeToEvent(E_NODE_UPDATE_SHOW,HANDLER(SceneView,HandleNodeUpdateShow));
}

void SceneView::add_node_to_tree(QTreeWidgetItem* parent,Node* pNode)
{
	QTreeWidgetItem* item = new QTreeWidgetItem(QStringList(pNode->GetName().CString()));
	if(parent == NULL)
	{
		sceneRootNode_ = item;
		sceneRootNode_->setText(0,"SceneRoot");
		sceneRootNode_->setIcon(0,QIcon(":/Images/Components/Root.png"));
		addTopLevelItem(sceneRootNode_);
	}
	else
	{
		parent->addChild(item);
	}

	item->setWhatsThis(0,QString::number(pNode->GetID()));

	//设置标记
	if(update_node_icon(item,pNode) == true)
	{
		for(int i = 0;i < pNode->GetNumChildren();i ++)
		{
			add_node_to_tree(item,pNode->GetChild(i));
		}
	}
}

bool SceneView::update_node_icon(QTreeWidgetItem* item,Node* pNode)
{
	if(pNode->GetComponent<StaticModel>() != NULL)
	{
		item->setIcon(0,QIcon(":/Images/Components/StaticModel.png"));
		item->setData(0,Qt::UserRole,QString("StaticModel") + pNode->GetID());
	}
	else if(pNode->GetComponent<AnimatedModel>() != NULL)
	{
		item->setIcon(0,QIcon(":/Images/Components/AnimatedModel.png"));
		item->setData(0,Qt::UserRole,"AnimatedModel");
		//动画节点跳出循环
		return false;
	}
	else if(pNode->GetComponent<Terrain>() != NULL)
	{
		item->setIcon(0,QIcon(":/Images/Components/Terrain.png"));
		return false;
	}
	else if(pNode->GetComponent<Skybox>() != NULL)
	{
		item->setIcon(0,QIcon(":/Images/Components/Skybox.png"));
	}
	else if(pNode->GetComponent<Light>() != NULL)
	{
		item->setIcon(0,QIcon(":/Images/Components/Light.png"));
	}
	else if(pNode->GetComponent<PhysicsWorld>() != NULL)
	{
		item->setIcon(0,QIcon(":/Images/Components/PhysicsWorld.png"));
	}
	else if(pNode->GetComponent<Camera>() != NULL)
	{
		item->setIcon(0,QIcon(":/Images/Components/Camera.png"));
	}
	else if(pNode->GetComponent<Zone>() != NULL)
	{
		item->setIcon(0,QIcon(":/Images/Components/Zone.png"));
	}
	else if(pNode == EditorRoot::Instance()->scene_)
	{
		item->setIcon(0,QIcon(":/Images/Components/Root.png"));
	}
	else if(pNode == EditorRoot::Instance()->GetGizmo()->GetNode())
	{
		this->removeItemWidget(item,0);
		return false;
	}
	else
	{
		//未知
		item->setIcon(0,QIcon(":/Images/Components/Unknown.png"));
	}

	return true;
}

void SceneView::selectionChanged()
{
	if(bSelectTrigger == false)
		return;

	QList<QTreeWidgetItem*> list = this->selectedItems();
	if(list.size() == 0)
		return;

	QTreeWidgetItem* selitem = list[0];
	Node* pNode = get_node_by_item(selitem);
	if(pNode != NULL)
	{
		String nodeName = pNode->GetName();

		on_node_select(pNode);
	}
}

void SceneView::on_node_select(Node* pNode)
{
	EditorRoot::Instance()->OnNodeSelect(pNode);
}

//双击摄像机跳转
void SceneView::itemDoubleClicked(QTreeWidgetItem *item, int column)
{
	Node* pNode = get_node_by_item(item);
	if(pNode != NULL)
	{
		Node* cameraNode_ = EditorRoot::Instance()->cameraNode_;
		//focus
		Camera* pCamera = cameraNode_->GetComponent<Camera>();
		if(pCamera)
		{
			Ray cameraRay = pCamera->GetScreenRay(0.5,0.5);
			Vector3 objPosition = pNode->GetWorldPosition();
			Vector3 dist = cameraRay.direction_ * (EditorRoot::Instance()->GetNodeSize(pNode) + 5.);	//当前Camera角度方向远离15单元 todo:根据物体大小进行调整

			cameraNode_->SetWorldPosition(objPosition - dist);
		}
	}
}

Node* SceneView::get_node_by_item(QTreeWidgetItem* item)
{
	int ID = item->whatsThis(0).toInt();
	if(ID > 0)
	{
		Node* pNode = EditorRoot::Instance()->scene_->GetNode(ID);
		return pNode;
	}
	else
	{
		return NULL;
	}
}

QTreeWidgetItem* SceneView::get_item_by_node(Node* pNode)
{
	QTreeWidgetItemIterator it(this);
	while(*it)
	{
		QTreeWidgetItem* item = *it;
		if(pNode == get_node_by_item(item))
		{
			return item;
		}

		++ it;
	}

	return NULL;
}
