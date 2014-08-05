#pragma once
#include <QWidget>
#include <QTreeWidget>
#include <QMenu>

#include "Object.h"

namespace Urho3D
{
	class Node;
}
using namespace Urho3D;

//场景树结构控件
class SceneView : public QTreeWidget , public Object
{
	Q_OBJECT;
	OBJECT(SceneView);
public:
	SceneView(QWidget* parent,Context* context = NULL);
	void Init(Node* pSceneNode);
	void ClearAllSelection();
public Q_SLOTS:
	void selectionChanged();
	void itemDoubleClicked(QTreeWidgetItem *item, int column);
private:
	//记录几个固定的节点Node
	QTreeWidgetItem* sceneRootNode_;

	void on_node_select(Node* pNode);

	//从树节点找到Node
	Node* get_node_by_item(QTreeWidgetItem* item);
	//根据Node找到对应的树节点
	QTreeWidgetItem* get_item_by_node(Node* pNode);

	void add_node_to_tree(QTreeWidgetItem* parent,Node* pNode);

	//更新Node的图标显示，返回编辑器中是否编辑其子节点，如动画是一个整体，内部节点场景编辑器不允许编辑
	bool update_node_icon(QTreeWidgetItem* parent,Node* pNode);

	void HandleNodeAdded(StringHash eventType, VariantMap& eventData);
	void HandleNodeRemoved(StringHash eventType, VariantMap& eventData);
	void HandleNodeSelectionChange(StringHash eventType, VariantMap& eventData);
	void HandleNodeUpdateShow(StringHash eventType, VariantMap& eventData);
	void HandleSceneReset(StringHash eventType, VariantMap& eventData);

	bool bSelectTrigger;	//标记设置选择状态是否触发方法
protected:
	void mouseReleaseEvent(QMouseEvent *evt);
};
